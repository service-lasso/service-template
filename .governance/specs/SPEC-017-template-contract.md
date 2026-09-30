# SPEC-017 — versioned template admission contract

Status: active

## Intent

Define an owner-controlled, machine-verifiable contract for projects created from `service-template`.

## Requirements

- `AC-17.1`: A versioned JSON policy has a closed normalized file inventory containing path, SHA-256 digest and Git mode for every immutable baseline file, plus explicit quotas.
- `AC-17.2`: Contract verification binds the policy version and canonical policy digest to project provenance, rejects malformed provenance, and never approves changed runtime, scripts, hooks, provider/dependency declarations or artifact sources.
- `AC-17.3`: Only declared service identity and safe metadata/configuration differences are admitted. Arbitrary commands, URLs, secret values, and unknown manifest fields are rejected.
- `AC-17.4`: A local author-created project remains admissible with the template archive digest. A GitHub-derived identity is required only when provenance says it is GitHub-derived.
- `AC-17.5`: Tests exercise the real template inventory plus valid, altered, forbidden and malformed project fixtures.

## External dependency boundary

Core proposal `#1514` / `SPEC-002 AC-4CF.1` and Core `#1513` are reference material only. This policy is owned by `service-template`; Core must explicitly adopt a compatible parser and catalog pin before it may use this policy for admission.
