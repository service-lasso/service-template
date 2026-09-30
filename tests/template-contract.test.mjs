import assert from "node:assert/strict";
import { createHash } from "node:crypto";
import { cp, mkdtemp, readFile, rm, symlink, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join, resolve } from "node:path";
import { gunzipSync, gzipSync } from "node:zlib";
import { spawnSync } from "node:child_process";
import test from "node:test";

const root = resolve(import.meta.dirname, "..");
const verifier = join(root, "scripts", "verify-template-contract.mjs");
const policy = JSON.parse(await readFile(join(root, "template-contract.json"), "utf8"));
const digest = (value) => createHash("sha256").update(value).digest("hex");
test("locked template files use LF checkout bytes", () => {
  const attributes = spawnSync("git", ["-C", root, "check-attr", "eol", "--", ".github/branch-protection-checklist.md"], { encoding: "utf8" });
  assert.equal(attributes.status, 0, attributes.stderr);
  assert.match(attributes.stdout, /\.github\/branch-protection-checklist\.md: eol: lf/);
});
async function fixture() {
  const directory = await mkdtemp(join(tmpdir(), "template-contract-"));
  const project = join(directory, "project");
  await cp(root, project, { recursive: true, filter: (path) => ![".git", ".harness", "dist", "output", "node_modules"].some((name) => path.endsWith(`/${name}`) || path.endsWith(`\\${name}`)) });
  const tar = spawnSync("git", ["-C", root, "archive", "--format=tar", "HEAD"], { encoding: null });
  assert.equal(tar.status, 0, tar.stderr?.toString());
  const archive = join(directory, "service-template.tar.gz");
  await writeFile(archive, gzipSync(tar.stdout, { mtime: 0 }));
  const commit = spawnSync("git", ["-C", root, "rev-parse", "HEAD"], { encoding: "utf8" });
  assert.equal(commit.status, 0, commit.stderr);
  const candidate = join(directory, "template-candidate.json");
  const descriptor = { schemaVersion: 1, kind: "development-template-candidate", templateCommit: commit.stdout.trim(), templateVersion: policy.templateVersion, contractDigest: policy.contractDigest, archiveSha256: digest(await readFile(archive)) };
  await writeFile(candidate, `${JSON.stringify(descriptor, null, 2)}\n`);
  return { directory, project, archive, candidate, descriptor };
}
async function provenance(project, descriptor, origin = { kind: "local-archive", archiveSha256: descriptor.archiveSha256 }) {
  await writeFile(join(project, "template-provenance.json"), `${JSON.stringify({ schemaVersion: 1, templateRepository: "service-lasso/service-template", templateCommit: descriptor.templateCommit, templateVersion: descriptor.templateVersion, contractDigest: descriptor.contractDigest, origin }, null, 2)}\n`);
}
function run(fixture) { return spawnSync(process.execPath, [verifier, "--template-root", root, "--project-root", fixture.project, "--candidate", fixture.candidate, "--candidate-archive", fixture.archive], { encoding: "utf8" }); }
async function derivedFixture(t) { const value = await fixture(); t.after(() => rm(value.directory, { recursive: true, force: true })); await provenance(value.project, value.descriptor); return value; }
test("valid local archive project is admitted using the owner candidate tuple", async (t) => { const value = await derivedFixture(t); const result = run(value); assert.equal(result.status, 0, result.stderr); assert.match(result.stdout, /"verified": true/); });
test("safe identity, GitHub identity and example configuration changes are admitted", async (t) => { const value = await fixture(); t.after(() => rm(value.directory, { recursive: true, force: true })); const origin = { kind: "github-derived", repository: "example/lasso-weather" }; await provenance(value.project, value.descriptor, origin); const manifest = JSON.parse(await readFile(join(value.project, "service.json"), "utf8")); manifest.id = "weather-service"; manifest.name = "Weather Service"; manifest.meta.repository.url = "https://github.com/example/lasso-weather.git"; await writeFile(join(value.project, "service.json"), `${JSON.stringify(manifest, null, 2)}\n`); await writeFile(join(value.project, "config", "example.env"), "ECHO_MESSAGE=weather\n"); const result = run(value); assert.equal(result.status, 0, result.stderr); });
test("invented commit and archive digests are denied even when provenance is well formed", async (t) => { const value = await derivedFixture(t); const invented = { ...value.descriptor, templateCommit: "b".repeat(40), archiveSha256: "a".repeat(64) }; await writeFile(value.candidate, `${JSON.stringify(invented, null, 2)}\n`); await provenance(value.project, invented); const result = run(value); assert.notEqual(result.status, 0); assert.match(result.stderr, /candidate_binding/); });
test("provenance cannot self-report an archive digest different from the candidate", async (t) => { const value = await derivedFixture(t); await provenance(value.project, value.descriptor, { kind: "local-archive", archiveSha256: "a".repeat(64) }); const result = run(value); assert.notEqual(result.status, 0); assert.match(result.stderr, /origin/); });
test("a candidate archive with a forged executable mode is denied from archive metadata", async (t) => {
  const value = await derivedFixture(t);
  const tar = gunzipSync(await readFile(value.archive));
  tar.write("0000775\0", 100, "ascii");
  tar.fill(0x20, 148, 156);
  const checksum = tar.subarray(0, 512).reduce((sum, byte) => sum + byte, 0).toString(8).padStart(6, "0");
  tar.write(`${checksum}\0 `, 148, "ascii");
  await writeFile(value.archive, gzipSync(tar, { mtime: 0 }));
  value.descriptor.archiveSha256 = digest(await readFile(value.archive));
  await writeFile(value.candidate, `${JSON.stringify(value.descriptor, null, 2)}\n`);
  await provenance(value.project, value.descriptor);
  const result = run(value); assert.notEqual(result.status, 0); assert.match(result.stderr, /candidate_archive/);
});
test("altered executable baseline is denied", async (t) => { const value = await derivedFixture(t); await writeFile(join(value.project, "runtime", "linux", "echo-service.sh"), "#!/usr/bin/env bash\necho altered\n"); const result = run(value); assert.notEqual(result.status, 0); assert.match(result.stderr, /immutable_baseline/); });
test("arbitrary artifact source change is denied", async (t) => { const value = await derivedFixture(t); const manifest = JSON.parse(await readFile(join(value.project, "service.json"), "utf8")); manifest.artifact.source.repo = "attacker/example"; await writeFile(join(value.project, "service.json"), `${JSON.stringify(manifest, null, 2)}\n`); const result = run(value); assert.notEqual(result.status, 0); assert.match(result.stderr, /forbidden_manifest_change/); });
test("secret-like configuration is denied", async (t) => { const value = await derivedFixture(t); await writeFile(join(value.project, "config", "example.env"), "API_TOKEN=not-allowed\n"); const result = run(value); assert.notEqual(result.status, 0); assert.match(result.stderr, /unsafe_configuration/); });
test("arbitrary configuration URL is denied", async (t) => { const value = await derivedFixture(t); await writeFile(join(value.project, "config", "example.env"), "UPSTREAM=https://attacker.invalid/payload\n"); const result = run(value); assert.notEqual(result.status, 0); assert.match(result.stderr, /unsafe_configuration/); });
test("malformed provenance is denied", async (t) => { const value = await fixture(); t.after(() => rm(value.directory, { recursive: true, force: true })); await writeFile(join(value.project, "template-provenance.json"), "{bad json"); const result = run(value); assert.notEqual(result.status, 0); assert.match(result.stderr, /malformed_provenance/); });
async function assertLinkDenied(t, name, type) {
  const value = await derivedFixture(t);
  try { await symlink(type === "dir" ? join(value.project, "runtime") : join(value.project, "README.md"), join(value.project, name), type); } catch (error) { if (["EPERM", "EACCES", "ENOTSUP"].includes(error.code)) return t.skip(`link creation is unavailable: ${error.code}`); throw error; }
  const result = run(value); assert.notEqual(result.status, 0); assert.match(result.stderr, /project_object/);
}
test("a file symlink is denied before allowlist and content reads", async (t) => assertLinkDenied(t, "forged-file", "file"));
test("a directory symlink is denied before traversal", async (t) => assertLinkDenied(t, "forged-directory", "dir"));
