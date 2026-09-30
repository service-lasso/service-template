import { createHash } from "node:crypto";
import { lstat, readdir, readFile } from "node:fs/promises";
import { join, relative, resolve, sep } from "node:path";
import { execFileSync } from "node:child_process";

class ContractError extends Error { constructor(code, message) { super(message); this.code = code; } }
const sha256 = (value) => createHash("sha256").update(value).digest("hex");
const canonical = (value) => Array.isArray(value) ? value.map(canonical) : value && typeof value === "object" ? Object.fromEntries(Object.keys(value).sort().map((key) => [key, canonical(value[key])])) : value;
function fail(code, message) { throw new ContractError(code, message); }
function parseArgs(args) {
  const out = {};
  if (args.length % 2) fail("usage", "Verifier arguments must be option/value pairs.");
  for (let index = 0; index < args.length; index += 2) {
    if (!new Set(["--template-root", "--project-root", "--candidate", "--candidate-archive"]).has(args[index]) || out[args[index]]) fail("usage", "Unknown or duplicate verifier argument.");
    out[args[index]] = args[index + 1];
  }
  if (Object.keys(out).length !== 4 || !Object.values(out).every(Boolean)) fail("usage", "Usage: verify-template-contract.mjs --template-root <path> --project-root <path> --candidate <owner-controlled-descriptor> --candidate-archive <immutable-template-archive>");
  return Object.fromEntries(Object.entries(out).map(([key, value]) => [key.slice(2).replaceAll("-", ""), resolve(value)]));
}
function trackedModes(root) {
  try {
    const rows = execFileSync("git", ["-C", root, "ls-files", "-s"], { encoding: "utf8", stdio: ["ignore", "pipe", "ignore"] }).trim().split("\n");
    return new Map(rows.filter(Boolean).map((row) => { const [metadata, path] = row.split("\t"); return [path, metadata.split(" ")[0].slice(-4)]; }));
  } catch { return new Map(); }
}
async function regularFile(path, code, message) {
  let info;
  try { info = await lstat(path); } catch { fail(code, message); }
  if (!info.isFile() || info.isSymbolicLink()) fail(code, message);
  return info;
}
async function listRegularFiles(root, code) {
  async function visit(directory) {
    let directoryInfo;
    try { directoryInfo = await lstat(directory); } catch { fail(code, `Cannot inspect project path: ${relative(root, directory) || "."}`); }
    if (!directoryInfo.isDirectory() || directoryInfo.isSymbolicLink()) fail(code, `Links and non-directory objects are forbidden: ${relative(root, directory) || "."}`);
    const entries = await readdir(directory, { withFileTypes: true });
    const files = [];
    for (const entry of entries) {
      const absolute = join(directory, entry.name);
      const path = relative(root, absolute).split(sep).join("/");
      if (new Set([".git", ".harness", "dist", "output", "node_modules"]).has(entry.name)) continue;
      const info = await lstat(absolute);
      if (info.isSymbolicLink() || (!info.isDirectory() && !info.isFile())) fail(code, `Links and non-regular objects are forbidden: ${path}`);
      if (info.isDirectory()) files.push(...await visit(absolute));
      else files.push({ path, absolute, size: info.size, mode: (info.mode & 0o777).toString(8).padStart(4, "0") });
    }
    return files;
  }
  return (await visit(root)).sort((left, right) => left.path.localeCompare(right.path));
}
async function readRegular(path, code, message) { await regularFile(path, code, message); return readFile(path); }
async function readJson(path, code) { try { return JSON.parse((await readRegular(path, code, `Malformed JSON: ${relative(process.cwd(), path)}`)).toString("utf8")); } catch (error) { if (error instanceof ContractError) throw error; fail(code, `Malformed JSON: ${relative(process.cwd(), path)}`); } }
function assertDigest(policy) {
  const unsigned = { ...policy }; delete unsigned.contractDigest;
  if (!/^[a-f0-9]{64}$/.test(policy.contractDigest) || sha256(JSON.stringify(canonical(unsigned))) !== policy.contractDigest) fail("contract_digest", "Template contract digest does not bind its policy and inventory.");
}
function archiveEntries(archive) {
  let output;
  try { output = execFileSync("tar", ["-tzvf", archive], { encoding: "utf8", stdio: ["ignore", "pipe", "pipe"] }); } catch { fail("candidate_archive", "Candidate archive cannot be listed."); }
  const entries = new Map();
  for (const line of output.split(/\r?\n/).filter(Boolean)) {
    const fields = line.trim().split(/\s+/);
    const rawPath = fields.at(-1);
    const path = rawPath.replace(/^\.\//, "").replaceAll("\\", "/");
    if (!path || path.endsWith("/")) continue;
    if (!line.startsWith("-")) fail("candidate_archive", `Candidate archive contains a link or non-regular member: ${path}`);
    if (!/^(?!\/)(?!.*(?:^|\/)\.\.(?:\/|$))[A-Za-z0-9._@/\\-]+$/.test(path) || entries.has(path)) fail("candidate_archive", "Candidate archive has an unsafe or duplicate member path.");
    // Git archive's Windows tar writer represents regular files as 0664/0775.
    // Preserve the staged Git regular/executable class across archive formats.
    entries.set(path, line.slice(1, 10).includes("x") ? "0755" : "0644");
  }
  return entries;
}
function archiveBytes(archive, path) { try { return execFileSync("tar", ["-xOzf", archive, path], { encoding: null, stdio: ["ignore", "pipe", "pipe"] }); } catch { fail("candidate_archive", `Candidate archive cannot read ${path}.`); } }
function candidateSchema(candidate) {
  if (Object.keys(candidate).sort().join(",") !== "archiveSha256,contractDigest,kind,schemaVersion,templateCommit,templateVersion" || candidate.schemaVersion !== 1 || candidate.kind !== "development-template-candidate" || !/^[a-f0-9]{40}$/.test(candidate.templateCommit) || !/^[a-f0-9]{64}$/.test(candidate.archiveSha256) || !/^[a-f0-9]{64}$/.test(candidate.contractDigest) || typeof candidate.templateVersion !== "string") fail("candidate", "Candidate descriptor must use the closed immutable-candidate schema.");
}
function diffPointers(baseline, candidate, path = "") {
  if (JSON.stringify(baseline) === JSON.stringify(candidate)) return [];
  if (!baseline || !candidate || typeof baseline !== "object" || typeof candidate !== "object" || Array.isArray(baseline) || Array.isArray(candidate)) return [path || "/"];
  return [...new Set([...Object.keys(baseline), ...Object.keys(candidate)].flatMap((key) => diffPointers(baseline[key], candidate[key], `${path}/${key.replaceAll("~", "~0").replaceAll("/", "~1")}`)))];
}
async function verify({ templateroot: templateRoot, projectroot: projectRoot, candidate: candidatePath, candidatearchive: candidateArchive }) {
  const policy = await readJson(join(templateRoot, "template-contract.json"), "malformed_contract");
  const candidate = await readJson(candidatePath, "candidate");
  await regularFile(candidateArchive, "candidate_archive", "Candidate archive must be a regular file.");
  candidateSchema(candidate);
  if (candidate.templateCommit !== execFileSync("git", ["-C", templateRoot, "rev-parse", "HEAD"], { encoding: "utf8" }).trim()) fail("candidate_binding", "Candidate descriptor does not bind this immutable template checkout.");
  if (sha256(await readRegular(candidateArchive, "candidate_archive", "Candidate archive must be a regular file.")) !== candidate.archiveSha256) fail("candidate_binding", "Candidate archive digest does not match the owner-controlled descriptor.");
  if (policy.schemaVersion !== 1 || !/^\d+\.\d+\.\d+(?:-[a-z0-9.]+)?$/.test(policy.templateVersion)) fail("contract_schema", "Unsupported contract schema or template version.");
  assertDigest(policy);
  if (candidate.templateVersion !== policy.templateVersion || candidate.contractDigest !== policy.contractDigest) fail("candidate_binding", "Candidate descriptor does not bind this template policy.");
  if (!Array.isArray(policy.inventory) || policy.inventory.length === 0 || policy.inventory.length > policy.quotas.maximumFiles) fail("inventory", "Contract inventory is missing or exceeds its quota.");
  const locked = new Map(policy.inventory.map((entry) => [entry.path, entry]));
  if (locked.size !== policy.inventory.length) fail("inventory", "Contract inventory contains duplicate paths.");
  const archive = archiveEntries(candidateArchive);
  const expectedArchivePaths = new Set([...locked.keys(), "template-contract.json"]);
  if (archive.size !== expectedArchivePaths.size || [...archive.keys()].some((path) => !expectedArchivePaths.has(path))) fail("candidate_archive", "Candidate archive does not have the closed template inventory.");
  const templateModes = trackedModes(templateRoot);
  for (const entry of policy.inventory) {
    if (!/^(?!\/)(?!.*(?:^|\/)\.\.(?:\/|$))[A-Za-z0-9._@/\\-]+$/.test(entry.path) || !/^[a-f0-9]{64}$/.test(entry.sha256) || !/^[0-7]{4}$/.test(entry.mode)) fail("inventory", "Contract inventory entry is malformed.");
    const file = join(templateRoot, entry.path);
    if (sha256(await readRegular(file, "template_drift", `Canonical template drifted at ${entry.path}.`)) !== entry.sha256 || templateModes.get(entry.path) !== entry.mode) fail("template_drift", `Canonical template drifted at ${entry.path}. Regenerate and review the contract.`);
    if (archive.get(entry.path) !== entry.mode || sha256(archiveBytes(candidateArchive, entry.path)) !== entry.sha256) fail("candidate_archive", `Candidate archive does not preserve immutable bytes and mode for ${entry.path}.`);
  }
  const projectFiles = await listRegularFiles(projectRoot, "project_object");
  if (projectFiles.length > policy.quotas.maximumFiles) fail("quota", "Project exceeds maximum file count.");
  const allowed = new Set([policy.authoring.provenanceFile, ...policy.authoring.configuration.allowedPaths, "template-contract.json", ...locked.keys()]);
  for (const file of projectFiles) if (!allowed.has(file.path)) fail("forbidden_path", `Project adds forbidden path: ${file.path}`);
  const provenance = await readJson(join(projectRoot, policy.authoring.provenanceFile), "malformed_provenance");
  if (Object.keys(provenance).sort().join(",") !== policy.provenance.required.slice().sort().join(",")) fail("provenance_shape", "Provenance must use the closed schema.");
  if (provenance.schemaVersion !== 1 || provenance.templateRepository !== policy.provenance.templateRepository || provenance.templateCommit !== candidate.templateCommit || provenance.templateVersion !== candidate.templateVersion || provenance.contractDigest !== candidate.contractDigest) fail("provenance_binding", "Provenance is not bound to the supplied immutable template candidate.");
  if (!provenance.origin || !policy.provenance.originKinds.includes(provenance.origin.kind)) fail("origin", "Unsupported provenance origin.");
  if (provenance.origin.kind === "local-archive" && (Object.keys(provenance.origin).sort().join(",") !== "archiveSha256,kind" || provenance.origin.archiveSha256 !== candidate.archiveSha256)) fail("origin", "Local archive provenance must bind the supplied immutable candidate archive.");
  if (provenance.origin.kind === "github-derived" && (Object.keys(provenance.origin).sort().join(",") !== "kind,repository" || !/^[A-Za-z0-9_.-]+\/[A-Za-z0-9_.-]+$/.test(provenance.origin.repository))) fail("origin", "GitHub-derived provenance requires owner/repository identity.");
  if (projectFiles.reduce((total, file) => total + file.size, 0) > policy.quotas.maximumTotalBytes) fail("quota", "Project exceeds total byte quota.");
  for (const entry of policy.inventory) {
    const projectFile = join(projectRoot, entry.path);
    const info = await regularFile(projectFile, "missing_locked_file", `Project is missing locked file: ${entry.path}`);
    if (entry.path === policy.authoring.manifest.path || policy.authoring.configuration.allowedPaths.includes(entry.path)) continue;
    if (sha256(await readRegular(projectFile, "immutable_baseline", `Immutable baseline changed: ${entry.path}`)) !== entry.sha256 || (process.platform !== "win32" && (info.mode & 0o777).toString(8).padStart(4, "0") !== entry.mode)) fail("immutable_baseline", `Immutable baseline changed: ${entry.path}`);
  }
  const baselineManifest = await readJson(join(templateRoot, policy.authoring.manifest.path), "malformed_template_manifest");
  const projectManifest = await readJson(join(projectRoot, policy.authoring.manifest.path), "malformed_manifest");
  const changed = diffPointers(baselineManifest, projectManifest);
  if (changed.some((pointer) => !policy.authoring.manifest.allowedJsonPointers.some((allowedPointer) => pointer === allowedPointer || pointer.startsWith(`${allowedPointer}/`)))) fail("forbidden_manifest_change", `Manifest changes are limited to declared authoring fields: ${changed.join(", ")}`);
  if (provenance.origin.kind === "github-derived" && projectManifest?.meta?.repository?.url !== `https://github.com/${provenance.origin.repository}.git`) fail("github_identity", "GitHub provenance must match meta.repository.url.");
  let configBytes = 0;
  for (const path of policy.authoring.configuration.allowedPaths) {
    const content = (await readRegular(join(projectRoot, path), "unsafe_configuration", `Configuration file is not a regular file: ${path}`)).toString("utf8"); configBytes += Buffer.byteLength(content);
    if (new RegExp(policy.authoring.configuration.forbiddenNamePattern, "i").test(path) || new RegExp(policy.authoring.configuration.forbiddenValuePattern, "im").test(content)) fail("unsafe_configuration", `Configuration is an example-only surface and cannot contain secret material: ${path}`);
  }
  if (configBytes > policy.quotas.maximumConfigBytes) fail("quota", "Configuration exceeds byte quota.");
  return { verified: true, templateVersion: policy.templateVersion, contractDigest: policy.contractDigest, lockedFiles: policy.inventory.length, provenance: provenance.origin.kind };
}
try { console.log(JSON.stringify(await verify(parseArgs(process.argv.slice(2))), null, 2)); }
catch (error) { console.error(JSON.stringify({ verified: false, code: error.code || "internal", message: error.message }, null, 2)); process.exitCode = 1; }
