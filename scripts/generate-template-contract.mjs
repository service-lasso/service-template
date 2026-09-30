import { createHash } from "node:crypto";
import { readdir, readFile, stat, writeFile } from "node:fs/promises";
import { join, relative } from "node:path";
import { execFileSync } from "node:child_process";

const root = process.cwd();
const policyPath = join(root, "template-contract.json");
const excluded = new Set([".git", "dist", "output", "node_modules", "template-contract.json"]);

async function files(directory) {
  const entries = await readdir(directory, { withFileTypes: true });
  const result = [];
  for (const entry of entries) {
    if (excluded.has(entry.name)) continue;
    const absolute = join(directory, entry.name);
    if (entry.isDirectory()) result.push(...await files(absolute));
    else if (entry.isFile()) result.push(absolute);
  }
  return result;
}

function digest(value) { return createHash("sha256").update(value).digest("hex"); }
function trackedModes() {
  const rows = execFileSync("git", ["ls-files", "-s"], { cwd: root, encoding: "utf8" }).trim().split("\n");
  return new Map(rows.filter(Boolean).map((row) => {
    const [metadata, path] = row.split("\t");
    return [path, metadata.split(" ")[0].slice(-4)];
  }));
}
function canonical(value) {
  if (Array.isArray(value)) return value.map(canonical);
  if (value && typeof value === "object") return Object.fromEntries(Object.keys(value).sort().map((key) => [key, canonical(value[key])]));
  return value;
}

const policy = JSON.parse(await readFile(policyPath, "utf8"));
const modes = trackedModes();
policy.inventory = [];
for (const file of await files(root)) {
  const info = await stat(file);
  const path = relative(root, file).replaceAll("\\", "/");
  policy.inventory.push({ path, sha256: digest(await readFile(file)), mode: modes.get(path) || (info.mode & 0o777).toString(8).padStart(4, "0") });
}
policy.inventory.sort((a, b) => a.path.localeCompare(b.path));
const unsigned = { ...policy, contractDigest: undefined };
delete unsigned.contractDigest;
policy.contractDigest = digest(JSON.stringify(canonical(unsigned)));
await writeFile(policyPath, `${JSON.stringify(policy, null, 2)}\n`);
console.log(`wrote ${policy.inventory.length} locked entries with digest ${policy.contractDigest}`);
