import assert from "node:assert/strict";
import { readFile } from "node:fs/promises";
import { join } from "node:path";
import { fileURLToPath } from "node:url";
import test from "node:test";

const root = fileURLToPath(new URL("..", import.meta.url));
const workflow = await readFile(join(root, ".github", "workflows", "development-candidate.yml"), "utf8");

test("manual prerelease publication keeps its write authority and identity narrowly scoped", () => {
  assert.match(workflow, /^permissions:\n  contents: read$/m);
  assert.match(workflow, /if: github\.event_name == 'workflow_dispatch'\n        permissions:\n          contents: write/);
  assert.doesNotMatch(workflow, /permissions:\n  contents: write/);
  assert.match(workflow, /template-v\$\(node -p.*templateVersion.*\)-\$\{GITHUB_SHA\}/);
  assert.match(workflow, /Refusing to replace existing development candidate release/);
  assert.match(workflow, /--prerelease/);
});

test("published candidate readback binds the original archive, contract, and provider immutability", () => {
  assert.match(workflow, /cp template-contract\.json candidate\/template-contract\.json/);
  assert.match(workflow, /sha256sum service-template\.tar\.gz template-candidate\.json template-contract\.json > SHA256SUMS/);
  assert.match(workflow, /candidate\/service-template\.tar\.gz[\s\\]+candidate\/template-candidate\.json[\s\\]+candidate\/template-contract\.json[\s\\]+candidate\/SHA256SUMS/);
  assert.match(workflow, /\.tag_name == \$tag and \.target_commitish == \$commit and \.prerelease == true and \.draft == false and \.immutable == true/);
  assert.match(workflow, /service-template\.tar\.gz"\) \| \.digest\] == \[\$archive\]/);
  assert.match(workflow, /template-contract\.json"\) \| \.digest\] == \[\$contract\]/);
});
