# Template contract

`template-contract.json` is the versioned owner policy for projects derived from this repository. The policy digest is SHA-256 over its recursively key-sorted JSON with `contractDigest` omitted. Any inventory, quota, authoring rule, or policy change changes that digest.

The closed inventory records every immutable baseline file as a normalized slash path, SHA-256 and Git-compatible mode. The verifier checks the canonical template first, then the candidate project. Runtime payloads, scripts, workflows/hooks, provider/dependency manifests, and artifact sources stay byte and mode identical.

Authors may change only the declared identity fields in `service.json`, the exact GitHub repository URL when their provenance says `github-derived`, and the listed example configuration files. Configuration rejects secret-like names/values and source URLs. All other added files and changed manifest paths are denied.

Each derived project adds `template-provenance.json`. It binds `templateCommit`, template version and contract digest. A `local-archive` origin supplies an archive SHA-256 and does not require a GitHub repository. A `github-derived` origin supplies `owner/repository` and must match `meta.repository.url`.

Run the verifier against an immutable template checkout and the candidate project:

```powershell
node .\scripts\verify-template-contract.mjs --template-root <template-root> --project-root <project-root>
```

Core admission is intentionally outside this repository. Core `#1513` and proposal `#1514` / `SPEC-002 AC-4CF.1` must explicitly adopt a compatible parser and curated catalog pin before Core can rely on this result.
