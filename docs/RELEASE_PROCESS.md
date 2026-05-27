# Release Process

Marut FCU uses a lightweight release process centered on `main`, tracked build outputs, and semantic versioning.

## Branch Model

- `main` is the integration branch and the source of tagged releases.
- feature work should happen on short-lived branches.
- direct commits to `main` should be avoided in normal collaboration.

Suggested branch prefixes:

- `fix/`
- `feature/`
- `docs/`
- `chore/`
- `refactor/`

## Versioning

The repository documentation and changelog use semantic versioning:

- `MAJOR`: breaking architectural or protocol changes
- `MINOR`: new features, targets, or telemetry capabilities
- `PATCH`: bug fixes, driver corrections, tuning fixes, and documentation repairs

## Release Checklist

1. Build every target intended for the release.
2. Confirm the corresponding `Debug/*.elf` outputs exist.
3. Update `CHANGELOG.md`.
4. Commit the changelog and any intentional tracked build-output changes.
5. Tag the release.
6. Publish the release notes with hardware assumptions and known limitations.

## Tracked Build Outputs

`Debug/` outputs are intentionally versioned in this repository.

- do not clean them immediately before a release
- do not treat unchanged tracked binaries as accidental
- if a target was rebuilt for the release, ensure the updated output is committed intentionally
