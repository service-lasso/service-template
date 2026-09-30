import { createHash } from "node:crypto";
import { access, readdir, readFile, stat } from "node:fs/promises";
import { join, relative, resolve, sep } from "node:path";

class ContractError extends Error { constructor(code, message) { super(message); this.code = code; } }
const sha256 = (value) => createHash("sha256").update(value).digest("hex");
const canonical = (value) => Array.isArray(value) ? value.map(canonical) : value && typeof value === "object" ? Object.fromEntries(Object.keys(value).sort().map((key) => [key, canonical(value[key])])) : value;
const exists = async (path) => access(path).then(() => true, () => false);
function parseArgs(args) {
  const out = {};
  for (let i = 0; i < args.length; i += 2) out[args[i]] = args[i + 1];
  if (!out["--template-root"] || !out["--project-root"]) throw new ContractError("usage", "Usage: verify-template-contract.mjs --template-root <path> --project-root <path>");
  return out;
}
async function listFiles(root) {
  const ignored = new Set([".git", "dist", "output", "node_modules"]);
  async function visit(directory) {
    const entries = await readdir(directory, { withFileTypes: true });
    const paths = [];
    for (const entry of entries) {
      if (ignored.has(entry.name)) continue;
      const file = join(directory, entry.name);
      if (entry.isDirectory()) paths.push(...await visit(file));
      else if (entry.isFile()) paths.push(relative(root, file).split(sep).join("/"));
    }
    return paths;
  }
  return (await visit(root)).sort();
}
function diffPointers(baseline, candidate, path = "") {
  if (JSON.stringify(baseline) === JSON.stringify(candidate)) return [];
  if (!baseline || !candidate || typeof baseline !== "object" || typeof candidate !== "object" || Array.isArray(baseline) || Array.isArray(candidate)) return [path || "/"];
  return [...new Set([...Object.keys(baseline), ...Object.keys(candidate)].flatMap((key) => diffPointers(baseline[key], candidate[key], `${path}/${key.replaceAll("~", "~0").replaceAll("/", "~1")}`)))];
}
function fail(code, message) { throw new ContractError(code, message); }
async function readJson(path, code) { try { return JSON.parse(await readFile(path, "utf8")); } catch { fail(code, `Malformed JSON: ${relative(process.cwd(), path)}`); } }
function assertDigest(policy) {
  const unsigned = { ...policy }; delete unsigned.contractDigest;
  const actual = sha256(JSON.stringify(canonical(unsigned)));
  if (!/^[a-f0-9]{64}$/.test(policy.contractDigest) || actual !== policy.contractDigest) fail("contract_digest", "Template contract digest does not bind its policy and inventory.");
}
async function verify({ templateRoot, projectRoot }) {
  const policy = await readJson(join(templateRoot, "template-contract.json"), "malformed_contract");
  if (policy.schemaVersion !== 1 || !/^\d+\.\d+\.\d+(?:-[a-z0-9.]+)?$/.test(policy.templateVersion)) fail("contract_schema", "Unsupported contract schema or template version.");
  assertDigest(policy);
  if (!Array.isArray(policy.inventory) || policy.inventory.length === 0 || policy.inventory.length > policy.quotas.maximumFiles) fail("inventory", "Contract inventory is missing or exceeds its quota.");
  const locked = new Map(policy.inventory.map((entry) => [entry.path, entry]));
  if (locked.size !== policy.inventory.length) fail("inventory", "Contract inventory contains duplicate paths.");
  for (const entry of policy.inventory) {
    if (!/^(?!\/)(?!.*(?:^|\/)\.\.(?:\/|$))[A-Za-z0-9._@/\\-]+$/.test(entry.path) || !/^[a-f0-9]{64}$/.test(entry.sha256) || !/^[0-7]{4}$/.test(entry.mode)) fail("inventory", "Contract inventory entry is malformed.");
    const file = join(templateRoot, entry.path);
    if (!(await exists(file)) || sha256(await readFile(file)) !== entry.sha256 || ((await stat(file)).mode & 0o777).toString(8).padStart(4, "0") !== entry.mode) fail("template_drift", `Canonical template drifted at ${entry.path}. Regenerate and review the contract.`);
  }
  const projectFiles = await listFiles(projectRoot);
  if (projectFiles.length > policy.quotas.maximumFiles) fail("quota", "Project exceeds maximum file count.");
  const allowed = new Set([policy.authoring.provenanceFile, ...policy.authoring.configuration.allowedPaths, "template-contract.json", ...locked.keys()]);
  for (const path of projectFiles) if (!allowed.has(path)) fail("forbidden_path", `Project adds forbidden path: ${path}`);
  const provenance = await readJson(join(projectRoot, policy.authoring.provenanceFile), "malformed_provenance");
  if (Object.keys(provenance).sort().join(",") !== policy.provenance.required.slice().sort().join(",")) fail("provenance_shape", "Provenance must use the closed schema.");
  if (provenance.schemaVersion !== 1 || provenance.templateRepository !== policy.provenance.templateRepository || !/^[a-f0-9]{40}$/.test(provenance.templateCommit) || provenance.templateVersion !== policy.templateVersion || provenance.contractDigest !== policy.contractDigest) fail("provenance_binding", "Provenance is not bound to this template contract.");
  if (!provenance.origin || !policy.provenance.originKinds.includes(provenance.origin.kind)) fail("origin", "Unsupported provenance origin.");
  if (provenance.origin.kind === "local-archive") {
    if (Object.keys(provenance.origin).sort().join(",") !== "archiveSha256,kind" || !/^[a-f0-9]{64}$/.test(provenance.origin.archiveSha256)) fail("origin", "Local archive provenance requires its SHA-256 digest.");
  }
  if (provenance.origin.kind === "github-derived") {
    if (Object.keys(provenance.origin).sort().join(",") !== "kind,repository" || !/^[A-Za-z0-9_.-]+\/[A-Za-z0-9_.-]+$/.test(provenance.origin.repository)) fail("origin", "GitHub-derived provenance requires owner/repository identity.");
  }
  const totalBytes = (await Promise.all(projectFiles.map(async (path) => (await stat(join(projectRoot, path))).size))).reduce((a, b) => a + b, 0);
  if (totalBytes > policy.quotas.maximumTotalBytes) fail("quota", "Project exceeds total byte quota.");
  for (const entry of policy.inventory) {
    const projectFile = join(projectRoot, entry.path);
    if (!(await exists(projectFile))) fail("missing_locked_file", `Project is missing locked file: ${entry.path}`);
    if (entry.path === policy.authoring.manifest.path || policy.authoring.configuration.allowedPaths.includes(entry.path)) continue;
    if (sha256(await readFile(projectFile)) !== entry.sha256 || ((await stat(projectFile)).mode & 0o777).toString(8).padStart(4, "0") !== entry.mode) fail("immutable_baseline", `Immutable baseline changed: ${entry.path}`);
  }
  const baselineManifest = await readJson(join(templateRoot, policy.authoring.manifest.path), "malformed_template_manifest");
  const projectManifest = await readJson(join(projectRoot, policy.authoring.manifest.path), "malformed_manifest");
  const changed = diffPointers(baselineManifest, projectManifest);
  if (changed.some((pointer) => !policy.authoring.manifest.allowedJsonPointers.some((allowedPointer) => pointer === allowedPointer || pointer.startsWith(`${allowedPointer}/`)))) fail("forbidden_manifest_change", `Manifest changes are limited to declared authoring fields: ${changed.join(", ")}`);
  if (provenance.origin.kind === "github-derived") {
    const url = projectManifest?.meta?.repository?.url;
    if (url !== `https://github.com/${provenance.origin.repository}.git`) fail("github_identity", "GitHub provenance must match meta.repository.url.");
  }
  let configBytes = 0;
  for (const path of policy.authoring.configuration.allowedPaths) {
    const file = join(projectRoot, path); const content = await readFile(file, "utf8"); configBytes += Buffer.byteLength(content);
    if (new RegExp(policy.authoring.configuration.forbiddenNamePattern, "i").test(path) || new RegExp(policy.authoring.configuration.forbiddenValuePattern, "im").test(content)) fail("unsafe_configuration", `Configuration is an example-only surface and cannot contain secret material: ${path}`);
  }
  if (configBytes > policy.quotas.maximumConfigBytes) fail("quota", "Configuration exceeds byte quota.");
  return { verified: true, templateVersion: policy.templateVersion, contractDigest: policy.contractDigest, lockedFiles: policy.inventory.length, provenance: provenance.origin.kind };
}
try { console.log(JSON.stringify(await verify(Object.fromEntries(Object.entries(parseArgs(process.argv.slice(2))).map(([key, value]) => [key === "--template-root" ? "templateRoot" : "projectRoot", resolve(value)]))), null, 2)); }
catch (error) { console.error(JSON.stringify({ verified: false, code: error.code || "internal", message: error.message }, null, 2)); process.exitCode = 1; }
