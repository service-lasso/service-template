# service-template

_Status: planning folder / review bundle_

This folder is the standalone local project area for the future `service-template` repo.

It currently contains the template-specific planning and OpenSpec material moved out of the donor-ref/harness staging areas so the template work can be reviewed on its own.

## Start here

Read in this order:

1. `docs/openspec-drafts/SPEC-SERVICE-TEMPLATE-REPO.md`
2. `docs/openspec-drafts/OPENSPEC-TRACKER.md`
3. `docs/reference/SERVICE-TEMPLATE-REPO.md`
4. `docs/reference/SERVICE-STRUCTURE-REVIEW.md`
5. `docs/reference/PROPOSED-CODEBASE-STRUCTURE.md`
6. `docs/reference/DECISION-CONTEXT.md`
7. `docs/reference/shared-runtime/QUESTION-LIST-AND-CODE-VALIDATION.md`
8. `docs/reference/shared-runtime/ARCHITECTURE-DECISIONS.md`
9. `docs/reference/shared-runtime/SERVICE-MANAGER-BEHAVIOR.md`
10. `docs/reference/adjacent/SPEC-SERVICE-LASSO-HARNESS.md`
11. `docs/reference/EXAMPLE-REPO-TREE.md`
12. `docs/reference/EXAMPLE-service.json`
13. `docs/reference/EXAMPLE-service-harness.json`
14. `docs/reference/EXAMPLE-verify.ps1`
15. `docs/reference/EXAMPLE-verify.sh`

## What is here

### OpenSpec drafts
- `docs/openspec-drafts/SPEC-SERVICE-TEMPLATE-REPO.md`
- `docs/openspec-drafts/OPENSPEC-TRACKER.md`

### Supporting reference/planning docs
- `docs/service-contract.md`
- `docs/service-json-reference.md`
- `docs/packaging.md`
- `docs/validation.md`
- `docs/reference/SERVICE-TEMPLATE-REPO.md`
- `docs/reference/SERVICE-STRUCTURE-REVIEW.md`
- `docs/reference/PROPOSED-CODEBASE-STRUCTURE.md`
- `docs/reference/DECISION-CONTEXT.md`
- `docs/reference/shared-runtime/QUESTION-LIST-AND-CODE-VALIDATION.md`
- `docs/reference/shared-runtime/ARCHITECTURE-DECISIONS.md`
- `docs/reference/shared-runtime/SERVICE-MANAGER-BEHAVIOR.md`
- `docs/reference/adjacent/SPEC-SERVICE-LASSO-HARNESS.md`
- `docs/reference/EXAMPLE-REPO-TREE.md`
- `docs/reference/EXAMPLE-service.json`
- `docs/reference/EXAMPLE-service-harness.json`
- `docs/reference/EXAMPLE-verify.ps1`
- `docs/reference/EXAMPLE-verify.sh`

## Purpose

This repo is intended to become the canonical starting point for Service Lasso service repos.

Its role is to define:
- one-service-per-repo expectations
- service repo layout
- service author documentation expectations
- sample service expectations
- packaging/release expectations
- validation-harness integration expectations

## Current status

This is now a first-pass starter skeleton, not just a planning folder.

The folder now includes:
- template-specific specs/design notes
- copied shared runtime/reconciliation context needed for template decisions
- copied adjacent harness-spec context needed for validation integration decisions
- actual starter repo files (`service.json`, `verify/`, `scripts/`, `runtime/`, `config/`, `.github/workflows/`)
- a packaged first-pass sample artifact at `dist/echo-service-win32.zip`
- a starter multi-OS GitHub Actions workflow that packages release archives and runs basic tests
