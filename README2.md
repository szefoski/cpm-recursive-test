# CPM Features Guide (full)

This README replaces the previous `README.md` and focuses entirely on CPM usage in this repository. It documents how CPM is configured and used here, the exact features exercised, practical workflows (including patching and lock-file generation), presets and automation, and troubleshooting guidance. The goal is to be a complete reference for contributors and CI.

Note: CPM can be used without package lock files — the lock mechanism is optional. Use a lock file when you need reproducible transitive resolutions across developers and CI.

## Table of Contents

- Introduction
- Quick Start
- CPM Setup (vendored / patched CPM + cache)
- Package Declaration Pattern (CPMDeclarePackage vs CPMAddPackage)
- Advanced Options (CUSTOM_CACHE_KEY, GIT_SHALLOW, EXCLUDE_FROM_ALL)
- Patching (PATCHES) — workflow and examples
- Lock Files (CPMUsePackageLock + cpm-update-package-lock)
- Presets & Automation (CMakePresets.json)
- FAQ & Troubleshooting
- References

---

## Introduction

This repository demonstrates a practical, production-ready approach to using CPM.cmake to manage external C++ dependencies. It shows how to:

- Use external source caching for offline builds and faster re-configures
- Declare package requirements in libraries
- Request declared packages in components
- Apply patches to downloaded dependencies and cache patched sources
- Use a package lock file at the application level for reproducible builds
- Optimize downloads and builds with `GIT_SHALLOW` and `EXCLUDE_FROM_ALL`
- Automate configure/build/test and lock-file updates using modern `CMakePresets.json` workflows


Everything shown here is intentionally explicit so you can copy the patterns directly into your projects.

## Quick Start

Recommended (one-line workflows using presets):

```bash
# Full debug workflow: configure, build, test
cmake --workflow --preset debug-workflow

# Full release workflow: configure, build, test
cmake --workflow --preset release-workflow
```

Traditional step-by-step:

```bash
# Configure (without tests)
cmake --preset release

# Build
cmake --build --preset release

# Run
./build-release/src/main
```

Generate or update the package lock file:

```bash
cmake --build --preset update-lock-file
```

First-time (from scratch):

```bash
rm -rf build .cpm-cache package-lock.cmake
cmake -B build -DBUILD_TESTING=ON
cmake --build build --target cpm-update-package-lock
cmake --build build
```

## CPM Setup (vendored / patched CPM + cache)

- Vendored CPM: this repository includes a patched CPM at `cmake/CPM-dz-patched.cmake`. The patch ensures `PATCHES` entries in generated `package-lock.cmake` remain portable (use `${CMAKE_SOURCE_DIR}`) instead of embedding absolute paths. Include it at the top of your root `CMakeLists.txt`:

```cmake
include(cmake/CPM-dz-patched.cmake)
```

- CPM source cache: define a shared cache for the repository to enable offline development and faster re-configures. The root `CMakeLists.txt` sets:

```cmake
set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cpm-cache" CACHE PATH "CPM source cache directory")
```

You can also expose or override the cache via `CMakePresets.json` environment variables so every developer and CI uses the same location.

## Package Declaration vs Consumption

Make the distinction explicit:

- `CPMDeclarePackage(...)` — declare an *external* package and its metadata (version, repository, options, cache key, patches). This is used by library authors to express requirements.
- `CPMAddPackage(NAME <name>)` — make a previously-declared package *available* to the current CMake scope and create the imported targets (e.g. `fmt::fmt`) that CMake targets can link against.

Both commands are part of the same dependency workflow but have different responsibilities and ideal placement in your project:

### `CPMDeclarePackage` — declare external packages (libraries)

- Purpose: register an external dependency and how it should be retrieved/built. Declarations live in library-level `CMakeLists.txt` (example: `hal/CMakeLists.txt`).
- What it does: records the package name, preferred version or tag, optional `CUSTOM_CACHE_KEY`, any `PATCHES`, and flags such as `GIT_SHALLOW` and `EXCLUDE_FROM_ALL`.

Example (declaration):

```cmake
CPMDeclarePackage(
  fmt
  NAME fmt
  VERSION 12.1.0
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 12.1.0
  CUSTOM_CACHE_KEY "12.1.0"
  OPTIONS "FMT_INSTALL YES"
  SYSTEM YES
  GIT_SHALLOW YES
  EXCLUDE_FROM_ALL YES
)
```

Notes for declarations:

- Place declarations in library or component root `CMakeLists.txt` to express the requirement once.
- Prefer `CUSTOM_CACHE_KEY` for released versions; use a `-patched` suffix for patched variants.
- Use `GIT_SHALLOW` and `EXCLUDE_FROM_ALL` to reduce download size and avoid building unnecessary targets.
- Do not commit lock files in library directories — the application controls locking.

### `CPMAddPackage` — make package available for consumption (components/targets)

- Purpose: request that a declared package be prepared and its CMake imported targets be created so your targets can link to them.
- What it does: if the package was declared earlier (in the same configure or by a parent `CMakeLists.txt`), CPM will ensure the source is available (from cache or download), apply patches if configured, and provide imported targets like `fmt::fmt` or `nlohmann_json::nlohmann_json` to `target_link_libraries`.

Example (consumption):

```cmake
CPMAddPackage(NAME fmt)
CPMAddPackage(NAME nlohmann_json)

add_library(spi src/spi.cpp)
target_link_libraries(spi PRIVATE fmt::fmt nlohmann_json::nlohmann_json)
```

How declaration + consumption interact with lock files:

- If the application calls `CPMUsePackageLock(package-lock.cmake)`, CPM resolves `CPMAddPackage()` requests using the locked declarations (exact `GIT_TAG`, `CUSTOM_CACHE_KEY`, and `PATCHES`) recorded in the lock file, ensuring consistent, reproducible builds.

Summary:

- `CPMDeclarePackage` = declare external dependency metadata (where/how to get it).
- `CPMAddPackage` = make the declared dependency available to your CMake targets (create imported targets to link against).

### CPM_SOURCE_CACHE

`CPM_SOURCE_CACHE` is a central CPM feature: it is the directory where CPM stores downloaded or checked-out source trees for external packages so they can be reused across runs, shared between projects, and restored in CI for fast, offline-capable builds.

Key points:

- Purpose: speed up configuration and builds, enable offline work, reduce bandwidth/latency in CI by reusing previously downloaded sources.
- Layout: CPM stores packages under `<CPM_SOURCE_CACHE>/<package-name>/<cache-key>/...` where `cache-key` is either generated by CPM (based on repo URL, options and patches) or explicitly set via `CUSTOM_CACHE_KEY`.
- Behavior: when a package is requested, CPM prefers the cache entry if present; otherwise it clones/downloads into the cache and uses that copy. CPM uses short-hash bookkeeping and file locks to avoid races during concurrent configure/builds.

How to set the cache:

- Environment variable (recommended for CI or per-user):

```bash
export CPM_SOURCE_CACHE="$HOME/.cache/CPM"
cmake -S . -B build
```

- CMake cache variable (in `CMakeLists.txt` or on CLI):

```cmake
set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cpm-cache" CACHE PATH "CPM source cache directory")
```

```bash
cmake -S . -B build -DCPM_SOURCE_CACHE="${PWD}/.cpm-cache"
```

- CMake presets: set in `CMakePresets.json` environment so all contributors and CI reuse the same location.

Disabling the cache: set `CPM_SOURCE_CACHE` to an empty string to force fresh downloads each run.

Interaction with patches, cache keys and lock files:

- `PATCHES` and `CUSTOM_CACHE_KEY`: patched sources must be cached separately. Use `CUSTOM_CACHE_KEY` (for example `"3.11.3-patched"`) to make patched caches explicit and shareable.
- Lock files (`package-lock.cmake`) include cache metadata (such as `CUSTOM_CACHE_KEY`) so an application can reliably reproduce the same cached resolution across machines.

Offline builds & CI tips:

- Populate the cache once (developer machine or a CI warm-up job), then restore the cache for subsequent CI jobs to avoid redownloading.
- In CI, restore the cached folder before running `cmake` and save it back after the job completes.

Invalidation & updates:

- To force CPM to re-download or apply new patches, remove the specific package cache directory, e.g. `rm -rf .cpm-cache/nlohmann_json/3.11.3-patched`, or clear the whole cache.
- Alternatively, change `CUSTOM_CACHE_KEY` when applying patches to keep old and new caches side-by-side.

Best practices:

- Use a per-repo local cache for development (`${sourceDir}/.cpm-cache`) and a shared global cache for CI if appropriate.
- Add `.cpm-cache/` to `.gitignore` and use your CI's caching mechanism to store/restore the directory between jobs.
- Prefer `CUSTOM_CACHE_KEY` for released versions and append `-patched` for patched variants so caches are explicit and predictable.
- When changing patches or package options, either bump `CUSTOM_CACHE_KEY` or delete the affected cache entry and regenerate the lock file.

Example commands:

```bash
# Configure using a repo-local cache
cmake -S . -B build -DCPM_SOURCE_CACHE="${PWD}/.cpm-cache"

# Clear one package's cache
rm -rf .cpm-cache/nlohmann_json/3.11.3-patched

# Regenerate lock file after clearing cache
rm package-lock.cmake
cmake -B build
cmake --build build --target cpm-update-package-lock
```

### Note: multiple declarations / conflicts

CPM records packages the first time they are added. If the same `NAME` is declared or added again later with a different `VERSION` or `GIT_TAG`, CPM will not silently replace the existing entry — the original (first) declaration wins. If a later declaration/request asks for a newer version than the one already included, CPM prints a warning such as:

```
CPM: Requires a newer version of <PackageName> (<requested>) than currently included (<current>). 
```

In practice this means you can safely declare package requirements in libraries and let the application (root) decide the final resolution; CPM will warn if different parts of the tree request incompatible newer versions, but it will preserve the first resolved version.

If you need to force a different resolution, use the `FORCE` option or adjust the application's `package-lock.cmake` so the desired exact revision is chosen at the top-level.

## Advanced Options (used in this project)

These options are applied in `CPMDeclarePackage()` calls to control caching, downloads and build behavior:

- `CUSTOM_CACHE_KEY` — controls the directory name inside `CPM_SOURCE_CACHE`. Use a semantic version or `-patched` suffix for patched builds.

  Example: `CUSTOM_CACHE_KEY "3.11.3-patched"` for a patched `nlohmann/json`.

- `GIT_SHALLOW YES` — request shallow clones (fast, small downloads). Recommended for third-party dependencies.

- `EXCLUDE_FROM_ALL YES` — prevents building dependency tests/examples unless explicitly requested, significantly speeding up builds.

These flags together give fast, deterministic, and space-efficient dependency management in CI and on developer machines.

## Patching (PATCHES)

This repository demonstrates applying patches to downloaded dependencies. Patching is useful for small fixes, compatibility adjustments, or demonstrative changes.

How to declare patches (see `hal/CMakeLists.txt`):

```cmake
CPMDeclarePackage(
  nlohmann_json
  NAME nlohmann_json
  VERSION 3.11.3
  GITHUB_REPOSITORY nlohmann/json
  GIT_TAG v3.11.3
  CUSTOM_CACHE_KEY "3.11.3-patched"
  PATCHES
    "${CMAKE_CURRENT_LIST_DIR}/patches/nlohmann_json/01-add-custom-comment.patch"
    "${CMAKE_CURRENT_LIST_DIR}/patches/nlohmann_json/02-customize-namespace.patch"
  SYSTEM YES
  GIT_SHALLOW YES
  EXCLUDE_FROM_ALL YES
)
```

Patching workflow (add/remove patches and regenerate lock file):

1. Add patch files to `hal/patches/<package>/`.
2. Update `CPMDeclarePackage()` to include `PATCHES` and (optionally) bump `CUSTOM_CACHE_KEY`.
3. Remove cached package (to force re-download & patch application):

```bash
rm -rf .cpm-cache/nlohmann_json
```

4. Remove the old lock file and regenerate it:

```bash
rm package-lock.cmake
cmake -B build
cmake --build build --target cpm-update-package-lock
cmake --build build
```

Notes:

- If you don't set `CUSTOM_CACHE_KEY`, CPM will create a unique cache key that includes patches, and changing patches creates a different cache directory automatically.
- This repo uses `cmake/CPM-dz-patched.cmake` so that `PATCHES` entries written into `package-lock.cmake` are portable across checkouts — they reference `${CMAKE_SOURCE_DIR}` rather than absolute file system paths.

## Lock Files (CPMUsePackageLock)

### Overview

CPM supports package lock files similar to other package managers (Cargo, npm, poetry). A lock file records the exact resolution of all transitive dependencies (specific tags/commits and cache keys) so that every developer and CI run uses the same source revisions.

This repository uses an application-level lock file (`package-lock.cmake`) to lock the transitive graph for the final executable while libraries continue to declare version *requirements*.

### Philosophy: Libraries vs Applications

- **Libraries** (e.g. `hal/`, `upper_layer/`) declare version requirements using `CPMDeclarePackage()` but do not commit lock files. This keeps libraries flexible for downstream consumers.
- **Applications** (the root project) may create and commit a single `package-lock.cmake` that pins exact revisions for reproducible builds and CI stability.

### Comparison to Cargo (Rust)

| Feature | Cargo | CPM (This Project) |
|---------|-------|-------------------|
| Library dependencies | `Cargo.toml` with version requirements | `CPMDeclarePackage()` in library CMakeLists.txt |
| Lock files in libraries | ❌ Not committed (`Cargo.lock` typically lives with application) | ❌ Not created in libraries |
| Application lock file | ✅ `Cargo.lock` committed | ✅ `package-lock.cmake` committed (optional) |
| Lock file generation | Automatic on `cargo build` | `cmake --build build --target cpm-update-package-lock` |
| Update dependencies | `cargo update` | Delete lock + regenerate (see below) |

### How it works

1. Libraries call `CPMDeclarePackage()` to declare requirements (name, version range or tag).
2. The application includes CPM and calls `CPMUsePackageLock(package-lock.cmake)` to instruct CPM to prefer entries from the lock file when resolving packages.
3. When you generate a lock file, CPM writes explicit `CPMDeclarePackage()` invocations into `package-lock.cmake` that include exact `GIT_TAG`, `CUSTOM_CACHE_KEY`, and any `PATCHES` entries (this repository uses a patched CPM to ensure those `PATCHES` entries remain portable).

### Updating dependencies (regenerating lock)

Method 1: Update library requirements and regenerate lock

```bash
# 1. Edit versions in library `CPMDeclarePackage()` calls
rm package-lock.cmake
rm -rf .cpm-cache/<affected-packages>
cmake -B build
cmake --build build --target cpm-update-package-lock
```

Method 2: Edit the lock file directly (quick pins)

```bash
# Edit `package-lock.cmake` to change GIT_TAG or CUSTOM_CACHE_KEY
rm -rf build .cpm-cache/<package>
cmake -B build
```

### Best practices

- Commit `package-lock.cmake` for application projects to ensure reproducible CI and developer builds.
- Do not create lock files inside reusable libraries — let applications control locking.
- When adding or removing `PATCHES`, clear the relevant `.cpm-cache/` entries and regenerate the lock file so the lock reflects patched cache keys.
- Prefer `CUSTOM_CACHE_KEY` for stable release versions and use `-patched` suffix when applying project-specific patches.

### Benefits

1. **Reproducible builds** — exact same dependency revisions everywhere.
2. **Library flexibility** — libraries remain usable by different apps with different resolutions.
3. **Explicit updates** — dependency changes are visible in VCS diffs.
4. **CI parity** — lock files eliminate surprising differences in CI runs.

> Reminder: `CPMUsePackageLock()` is optional. CPM works without lock files by resolving requirements at configure time; use a lock file when you need strict reproducibility.

## Presets & Automation

This project includes a modern `CMakePresets.json` (schema version 10) that:

- Sets `CPM_SOURCE_CACHE` via environment for consistent caching across environments.
- Offers hidden base presets and `debug`/`release` variants with `with-tests` options.
- Includes workflow presets (`debug-workflow`, `release-workflow`) that run configure → build → test.
- Provides a build preset `update-lock-file` that runs the `cpm-update-package-lock` target.

Useful commands:

```bash
# Run full pipeline (configure, build, test)
cmake --workflow --preset debug-workflow

# Regenerate the lock file
cmake --build --preset update-lock-file
```

Using presets reduces configuration mistakes and ensures CI and local environments behave consistently.

## FAQ & Troubleshooting

- Q: Patch application fails (hunk FAILED).

  A: Verify your patch matches the exact `GIT_TAG` used. Test locally by entering the cached source and applying the patch manually:

```bash
cd .cpm-cache/<pkg>/<cache-key>/
patch -p1 < /path/to/patch
```

  If it fails, update the patch or change `GIT_TAG` to the commit that matches the patch context.

- Q: I changed patches but the build still uses the old code.

  A: Remove the cached package directory (`.cpm-cache/<pkg>/`) or change `CUSTOM_CACHE_KEY`, then regenerate the lock file and rebuild.

- Q: package-lock.cmake contains absolute paths for patches.

  A: This repo includes a patched CPM (`cmake/CPM-dz-patched.cmake`) specifically to avoid absolute paths and write portable `PATCHES` entries. Keep using the patched CPM until upstream CPM fixes this behavior.

- Q: Should libraries have lock files?

  A: No. In this model libraries declare requirements. The application is responsible for lock files to preserve flexibility for library consumers.

## References

- CPM: https://github.com/cpm-cmake/CPM.cmake
- This repo's patched CPM documentation: `cmake/CPM-dz-patched-README.md`
- Full project examples: `README.md` (archival) and `CMakePresets.json`

---

If you want, I can now:

- commit the new `README2.md` (and remove `README.md`) on branch `test-cpm-lock-file`, or
- rename `README2.md` to `README.md` and delete the old file, or
- make edits to any section (add more examples, screenshots, or shorten content).

Which would you like me to do next?