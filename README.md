# CPM.cmake Guide: Package Management Best Practices

A comprehensive guide to using CPM (CMake Package Manager) for C++ projects, demonstrating:
- Package lock files (Cargo/npm-style reproducible builds)
- Multi-project dependency management
- Library vs application architecture patterns
- Real-world C++23 layered architecture example

**Learn CPM through a working example** - This project serves as both documentation and reference implementation.

## 📑 Table of Contents

### Getting Started
- [📋 What is CPM?](#-what-is-cpm)
- [🎯 Quick Start](#-quick-start)
- [🛠️ Project Overview](#️-project-overview)

### CPM Core Concepts
- [🔒 Package Lock Mechanism](#-package-lock-mechanism)
  - [Philosophy: Libraries vs Applications](#philosophy-libraries-vs-applications)
  - [Comparison to Cargo (Rust)](#comparison-to-cargo-rust)
  - [How It Works](#how-it-works)
  - [Updating Dependencies](#updating-dependencies)
  - [Best Practices](#best-practices)
- [🎯 CMake Organization Patterns](#-cmake-organization-patterns)
  - [Application Level (Root)](#application-level-root)
  - [Library Level](#library-level)
  - [Component Level](#component-level)
  - [Key Patterns](#key-patterns)
- [🔧 Configuration Options](#-configuration-options)
  - [CPM Source Cache](#cpm-source-cache)
  - [Build Options](#build-options)
- [🩹 Patching Dependencies](#-patching-dependencies)

### Reference Implementation
- [🏗️ Architecture Principles](#️-architecture-principles)
- [📁 Project Structure](#-project-structure)
- [🔗 Dependency Chain](#-dependency-chain)
- [💡 Design Decisions](#-design-decisions)

### Practical Usage
- [🚀 Building](#-building)
- [🧪 Unit Testing](#-unit-testing)
- [📊 Expected Output](#-expected-output)

### Additional Resources
- [🛠️ Technologies](#️-technologies)
- [📚 Learn More](#-learn-more)
- [📝 License](#-license)
- [🤝 Contributing](#-contributing)

---

## 📋 What is CPM?

**CPM.cmake** is a modern CMake package manager that:
- ✅ Downloads dependencies automatically from Git repositories
- ✅ Integrates seamlessly with CMake (no separate tools)
- ✅ Supports source caching for offline builds
- ✅ Provides package lock files for reproducible builds
- ✅ Works with any CMake project (header-only or compiled)

### Why Use CPM?

**Traditional CMake dependency management:**
```cmake
# Manual approach - painful!
find_package(fmt REQUIRED)  # User must install fmt system-wide
find_package(nlohmann_json REQUIRED)  # Or use submodules, or...
```

**With CPM:**
```cmake
include(cmake/CPM.cmake)
CPMAddPackage("gh:fmtlib/fmt#12.1.0")  # Automatic download & build
CPMAddPackage("gh:nlohmann/json#v3.11.3")
```

**Benefits:**
- 🎯 No manual dependency installation
- 📦 Consistent versions across all developers
- 🔒 Lock files ensure reproducible builds
- 💾 Source cache enables offline development
- 🚀 Works on any platform (Windows, Linux, macOS)

---

## 🎯 Quick Start

### 1. Get CPM.cmake

**Download** (vendored, recommended):
```bash
mkdir cmake
wget -O cmake/CPM.cmake https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.0/CPM.cmake
```

**Or auto-download** (requires internet):
```cmake
set(CPM_DOWNLOAD_VERSION 0.42.0)
file(DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
  ${CMAKE_BINARY_DIR}/cmake/CPM.cmake
)
include(${CMAKE_BINARY_DIR}/cmake/CPM.cmake)
```

### 2. Add Dependencies

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.23)
project(MyProject)

include(cmake/CPM.cmake)

# Add packages
CPMAddPackage("gh:fmtlib/fmt#12.1.0")

add_executable(main main.cpp)
target_link_libraries(main fmt::fmt)
```

### 3. Build

```bash
cmake -B build
cmake --build build
./build/main
```

**That's it!** CPM automatically downloads fmt and makes it available.

---

## 🛠️ Project Overview

This repository demonstrates CPM usage through a real-world C++23 example:

**Architecture:**
- 🔧 **HAL Layer** (reusable library): Hardware abstraction components (spi, crypto)
- 📚 **Upper Layer** (reusable library): OS abstraction layer (osal)
- 🚀 **Application**: Final executable consuming the libraries

**CPM Features Demonstrated:**
- ✅ **Package lock files** - Application-level reproducibility (Cargo/npm-style)
- ✅ **Multi-project dependencies** - Libraries declare requirements independently
- ✅ **Transitive dependencies** - Automatic resolution through dependency chain
- ✅ **Dependency patching** - Apply custom fixes with PATCHES parameter
- ✅ **Source caching** - Offline builds after first download
- ✅ **CPMDeclarePackage + CPMAddPackage** - Advanced dependency patterns
- ✅ **EXCLUDE_FROM_ALL** - Build optimization for faster compilation

**Technologies Used:**
- **C++23** - Modern C++ with `std::println`
- **CMake 3.23+** - Build system
- **CPM.cmake v0.42.0** - Package manager
- **fmt 12.1.0** - Formatting library
- **nlohmann/json 3.11.3** - JSON library
- **Google Test 1.15.2** - Unit testing

---

## 🔒 Package Lock Mechanism

### Overview

CPM supports **package lock files** similar to Cargo (Rust), npm (Node.js), or poetry (Python).

```
Main Application
 +-> osal (OS Abstraction Layer)
      |-- fmt 12.1.0
      +-- crypto (HAL component)
           +-- spi (HAL component)
                |-- fmt 12.1.0
                +-- nlohmann/json 3.11.3
```

**Key Architecture Features:**
- **HAL Layer**: Hardware abstraction components (spi, crypto)
- **Upper Layer**: OS abstraction layer (osal)
- **External Dependencies**: Managed via CPM with lock file at application level
- **Local Components**: Organized by layer with add_subdirectory()

## 🛠️ Technologies

- **C++23** - Latest C++ standard with `std::println`
- **CMake 3.23+** - Modern build system
- **CPM.cmake v0.42.0** - External dependency management
- **fmt 12.1.0** - Fast formatting library
- **nlohmann/json 3.11.3** - JSON library
- **Google Test 1.15.2** - Unit testing framework

## 📁 Project Structure
```
cpm-recursive-test/
├── CMakeLists.txt              # Root configuration with package lock
├── package-lock.cmake          # Locked dependency versions (auto-generated)
├── cmake/
│   └── CPM.cmake              # Vendored CPM v0.42.0
├── src/
│   ├── CMakeLists.txt         # Main executable
│   └── main.cpp
├── hal/                       # Hardware Abstraction Layer (reusable library)
│   ├── CMakeLists.txt         # Declares requirements (no lock file)
│   ├── spi/                   # SPI HAL component
│   │   ├── CMakeLists.txt
│   │   ├── include/spi.hpp
│   │   ├── src/spi.cpp
│   │   └── unit_tests/spi_test.cpp
│   └── crypto/                # Crypto HAL component
│       ├── CMakeLists.txt
│       ├── include/crypto.hpp
│       ├── src/crypto.cpp
│       └── unit_tests/crypto_test.cpp
├── upper_layer/               # Upper Layer (reusable library)
│   ├── CMakeLists.txt         # Declares requirements (no lock file)
│   └── osal/                  # OS Abstraction Layer
│       ├── CMakeLists.txt
│       ├── include/osal.hpp
│       ├── src/osal.cpp
│       └── unit_tests/osal_test.cpp
└── .cpm-cache/                # Local dependency cache (gitignored)
```

## 🏗️ Architecture Principles

### Layered Organization
- **hal/**: Hardware abstraction components
  - `spi`: SPI interface (depends on fmt, nlohmann/json)
  - `crypto`: Cryptography (depends on spi)
- **upper_layer/**: High-level abstractions
  - `osal`: OS abstraction layer (depends on crypto, fmt)

### Naming Conventions
- **Namespaces**: Match directory structure (`hal::spi`, `hal::crypto`, `upper_layer::osal`)
- **Classes**: PascalCase matching component name (`Spi`, `Crypto`, `Osal`)
- **Files**: Lowercase matching component name (`spi.hpp`, `crypto.cpp`, `osal.cpp`)
- **Member variables**: Descriptive with trailing underscore (`spi_`, `crypto_`)

### Dependency Management Strategy
- **External dependencies** (fmt, nlohmann/json): Managed via CPM
  - Libraries (`hal/`, `upper_layer/`) declare version requirements
  - Application (root) locks specific versions via `package-lock.cmake`
  - Cached in `.cpm-cache/` directory
- **Local components**: Organized with `add_subdirectory()`
  - HAL components in `hal/`
  - Upper layer components in `upper_layer/`
  - Source directory in `src/`
## 🔒 Package Lock Mechanism

This project uses CPM's package lock file mechanism, following **Cargo (Rust) best practices**:

### Philosophy: Libraries vs Applications

**Libraries** (`hal/`, `upper_layer/`):
- ✅ Declare version **requirements** with `CPMDeclarePackage()`
- ❌ **NO** lock files
- Why: Allow consumers flexibility in dependency resolution

**Application** (root):
- ✅ Single `package-lock.cmake` locks all transitive dependencies
- ✅ Ensures **reproducible builds** across teams and CI/CD
- Why: Exact versions guarantee consistent behavior

### Comparison to Cargo (Rust)

| Feature | Cargo | CPM (This Project) |
|---------|-------|-------------------|
| Library dependencies | `Cargo.toml` with version requirements | `CPMDeclarePackage()` in library CMakeLists.txt |
| Lock files in libraries | ❌ Not committed (`.gitignore`) | ❌ Not created |
| Application lock file | ✅ `Cargo.lock` committed | ✅ `package-lock.cmake` committed |
| Lock file generation | Automatic on `cargo build` | `cmake --build build --target cpm-update-package-lock` |
| Update dependencies | `cargo update` | Delete lock + regenerate |

### How It Works

**1. Libraries declare requirements:**
```cmake
# hal/CMakeLists.txt
include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

CPMDeclarePackage(fmt
  NAME fmt
  VERSION 12.1.0  # Requirement, not locked version
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 12.1.0
  ...
)
```

**2. Application loads lock file:**
```cmake
# CMakeLists.txt (root)
include(cmake/CPM.cmake)
CPMUsePackageLock(package-lock.cmake)  # ← Lock file takes priority

add_subdirectory(hal)
add_subdirectory(upper_layer)
```

**3. Lock file contains exact versions:**
```cmake
# package-lock.cmake (auto-generated)
CPMDeclarePackage(fmt
  NAME fmt
  VERSION 12.1.0
  GIT_TAG 12.1.0          # ← Exact commit
  GITHUB_REPOSITORY fmtlib/fmt
  CUSTOM_CACHE_KEY 12.1.0 # ← Cache isolation
  ...
)
```

### Updating Dependencies

**Method 1: Update library requirements, regenerate lock**
```bash
# 1. Edit hal/CMakeLists.txt or upper_layer/CMakeLists.txt
#    Change VERSION in CPMDeclarePackage()

# 2. Clean and regenerate lock file
rm package-lock.cmake
rm -rf .cpm-cache/fmt .cpm-cache/nlohmann_json  # Clear affected packages
cmake -B build
cmake --build build --target cpm-update-package-lock
```

**Method 2: Edit lock file directly** (for quick pins)
```bash
# Edit package-lock.cmake directly
# Change VERSION or GIT_TAG

# Reconfigure
rm -rf build .cpm-cache/<package>
cmake -B build
```

### Best Practices

✅ **DO:**
- Commit `package-lock.cmake` to version control
- Regenerate lock file when adding/updating dependencies
- Use lock file in CI/CD for reproducible builds

❌ **DON'T:**
- Create lock files in library directories
- Manually write lock file from scratch (use `cpm-update-package-lock` target)
- Edit library CMakeLists.txt expecting immediate effect (lock file has priority)

### Benefits

1. **Reproducible Builds**: Everyone gets exact same dependency versions
2. **Library Flexibility**: Libraries can be consumed by different apps with different resolutions
3. **Explicit Updates**: Dependency changes are visible in version control
4. **Industry Standard**: Matches Cargo, npm, poetry, bundler patterns

## 🚀 Building

### Prerequisites

- CMake 3.23 or higher
- C++23 compatible compiler (GCC 14+, Clang 17+, MSVC 2022+)

### Build Commands

```bash
# Configure (lock file auto-used if present)
cmake -B build

# Build
cmake --build build

# Run
./build/src/main

# Run tests
cmake -B build -DBUILD_TESTING=ON
cmake --build build
cd build && ctest --output-on-failure
```

### First-Time Setup

```bash
# Clean build from scratch
rm -rf build .cpm-cache package-lock.cmake

# Configure (downloads dependencies)
cmake -B build -DBUILD_TESTING=ON

# Generate lock file
cmake --build build --target cpm-update-package-lock

# Build everything
cmake --build build
```

### Offline Build

After the first successful build, `.cpm-cache/` contains all external dependencies for offline builds.

---

## 🎯 CMake Organization Patterns

CPM provides flexible patterns for organizing dependencies in multi-project setups.

### Application Level (Root)

**Purpose:** Load lock file, aggregate subprojects

```cmake
# CMakeLists.txt (root)
cmake_minimum_required(VERSION 3.23)
project(MyApp)

# Package lock file - reproducible builds
include(cmake/CPM.cmake)
CPMUsePackageLock(package-lock.cmake)

# Add component layers
add_subdirectory(hal)
add_subdirectory(upper_layer)
add_subdirectory(src)
```

**Key Points:**
- ✅ Use `CPMUsePackageLock()` for reproducible builds
- ✅ Lock file contains exact versions of all transitive dependencies
- ✅ Only the application has a lock file (not libraries)

### Library Level

**Purpose:** Declare version requirements for reusable libraries

```cmake
# hal/CMakeLists.txt
cmake_minimum_required(VERSION 3.23)
project(hal VERSION 1.0.0)

include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

# Declare dependency requirements (not locked)
CPMDeclarePackage(fmt
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

# Add HAL components
add_subdirectory(spi)
add_subdirectory(crypto)
```

**Key Points:**
- ✅ Use `CPMDeclarePackage()` to specify requirements
- ✅ No lock file in libraries (flexibility for consumers)
- ❌ Don't use `CPMAddPackage()` at this level (only declare)

### Component Level

**Purpose:** Request already-declared dependencies

```cmake
# hal/spi/CMakeLists.txt
# Request external dependencies by name (versions from parent)
CPMAddPackage(NAME fmt)
CPMAddPackage(NAME nlohmann_json)

add_library(spi src/spi.cpp)
target_include_directories(spi PUBLIC include)
target_link_libraries(spi PRIVATE fmt::fmt nlohmann_json::nlohmann_json)
```

**Key Points:**
- ✅ Use `CPMAddPackage(NAME ...)` to request declared packages
- ✅ Version comes from parent's `CPMDeclarePackage()` or lock file
- ❌ Don't specify VERSION/GIT_TAG here (parent controls it)

### Key Patterns
- **CPMUsePackageLock** at root: Load lock file for reproducible builds
- **CPMDeclarePackage** in libraries: Declare version requirements
- **CPMAddPackage** in components: Request declared dependencies by NAME only
- **add_subdirectory**: Include local component layers and subdirectories
- **No lock files in libraries**: Only the final application has a lock file

## 💡 Design Decisions

### Why package lock file?
Following **Cargo (Rust) best practices**:
- **Reproducible builds**: Everyone gets exact same dependency versions
- **Library flexibility**: hal/upper_layer can be reused with different version resolutions
- **Explicit updates**: Dependency changes visible in version control
- **CI/CD consistency**: Lock file ensures identical builds across environments

### Why layered directory structure?
- Clear separation of concerns (HAL vs upper layer)
- Easy to navigate and understand
- Scalable for adding more components
- Matches common embedded/systems architecture patterns
- Each layer has its own CMakeLists.txt for organization

### Why EXCLUDE_FROM_ALL for dependencies?
**Problem**: Without `EXCLUDE_FROM_ALL`, building your project also builds:
- The dependency's library (needed ✓)
- The dependency's tests (not needed ✗)
- The dependency's examples (not needed ✗)
- The dependency's benchmarks (not needed ✗)

**How EXCLUDE_FROM_ALL works:**
```cmake
CPMDeclarePackage(
  GTest
  ...
  EXCLUDE_FROM_ALL YES  # ← This flag
)
```

**Behavior:**
- **Downloads** the package when `CPMAddPackage()` is called (always happens)
- **Builds only what you link to**: If you link to `gtest_main`, only `gtest_main` is built
- **Skips unnecessary targets**: GTest's own tests, samples, and benchmarks are NOT built
- **Faster builds**: Only compile what you actually use
- **Less disk space**: Fewer build artifacts

**Without EXCLUDE_FROM_ALL:**
```bash
cmake --build build
# Builds: your code + GTest lib + GTest's 50+ test executables
```

**With EXCLUDE_FROM_ALL YES:**
```bash
cmake --build build
# Builds: your code + GTest lib (only what you link against)
```

**Real-world impact:**
- Small libraries: 10-30% faster builds
- Large libraries (Boost, Qt): 50-80% faster builds
- Multiple dependencies: Compounds significantly

**Key insight:** `EXCLUDE_FROM_ALL` is about **build efficiency**, not about whether the package is downloaded or used. The package is always downloaded when `CPMAddPackage()` is called, but only necessary targets are compiled.

**Best practice recommendation:** 
- ✅ **Always use `EXCLUDE_FROM_ALL YES` for all third-party dependencies**
- Apply to fmt, nlohmann/json, GTest, and any other external libraries
- Only exception: When you explicitly want to build the dependency's own tests (very rare)
- This is a standard optimization that should be applied consistently

### Why GIT_SHALLOW for dependencies?

**What is Git shallow clone?**
- Normal clone: Downloads entire Git history (all commits, branches, tags)
- Shallow clone: Downloads only the latest commit or specific tag (much faster)

**Problem**: Downloading full Git history for dependencies is wasteful:
- ❌ Slower downloads (megabytes or gigabytes of history)
- ❌ More disk space consumed
- ❌ Unnecessary data (you only need one specific version)

**Solution with GIT_SHALLOW:**
```cmake
CPMDeclarePackage(fmt
  NAME fmt
  VERSION 12.1.0
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 12.1.0
  GIT_SHALLOW YES  # ← Only download this tag, not entire history
)
```

**Benefits:**
- ⚡ **Faster downloads** - 50-90% reduction in download time
- 💾 **Less disk space** - Often 10x smaller
- 🚀 **Faster CI/CD** - Quicker pipeline execution
- 🌐 **Better for slow connections** - Less data transfer

**Real-world impact:**
| Package | Full Clone | Shallow Clone | Speedup |
|---------|-----------|---------------|---------|
| fmt | ~8 MB | ~1 MB | 8x faster |
| nlohmann/json | ~4 MB | ~0.5 MB | 8x faster |
| GoogleTest | ~35 MB | ~3 MB | 11x faster |
| Boost | ~500 MB | ~50 MB | 10x faster |

**When NOT to use GIT_SHALLOW:**
- ❌ When you need Git history for development (e.g., debugging, contributing upstream)
- ❌ When using Git submodules that reference specific commits (not tags)
- ✅ **For dependencies: Always use it** (you just need the code, not history)

**Best practice recommendation:**
- ✅ **Always use `GIT_SHALLOW YES` for all third-party dependencies**
- Combine with `EXCLUDE_FROM_ALL YES` for maximum efficiency
- CPM v0.27+ enables shallow clones by default, but explicit is better

**Note:** CPM automatically enables shallow clones by default for Git-based dependencies. Being explicit with `GIT_SHALLOW YES` ensures consistent behavior across CPM versions.

### Why CUSTOM_CACHE_KEY?

**What is it?**
`CUSTOM_CACHE_KEY` controls how CPM caches downloaded dependencies in the `CPM_SOURCE_CACHE` directory.

**Problem**: By default, CPM uses Git repository URL + options as cache key:
- Same package from different sources (GitHub, GitLab, custom URL) creates separate caches
- Changing build options forces re-download even for same source version
- Wastes disk space with duplicate copies

**Default behavior without CUSTOM_CACHE_KEY:**
```bash
.cpm-cache/
├── fmt/HASH_A1B2C3/  # fmt from GitHub with option A
├── fmt/HASH_D4E5F6/  # fmt from GitHub with option B (duplicate!)
└── fmt/HASH_G7H8I9/  # fmt from GitLab (duplicate!)
```

**With CUSTOM_CACHE_KEY:**
```cmake
CPMDeclarePackage(fmt
  NAME fmt
  VERSION 12.1.0
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 12.1.0
  CUSTOM_CACHE_KEY "12.1.0"  # ← Simple version-based key
  OPTIONS "FMT_INSTALL YES"
)
```

**Result:**
```bash
.cpm-cache/
└── fmt/12.1.0/  # Single cache location regardless of options
```

**Benefits:**
- 💾 **Shared cache across projects** - Multiple projects using fmt 12.1.0 share one copy
- 🚀 **Faster configuration** - No re-downloads when changing build options
- 💿 **Less disk space** - One copy per version instead of per-configuration
- 🎯 **Predictable cache location** - Easy to find and manage

**Common patterns:**

**Version-based (recommended for released versions):**
```cmake
CUSTOM_CACHE_KEY "12.1.0"  # Use semantic version
```

**Commit-based (for specific commits):**
```cmake
CUSTOM_CACHE_KEY "abc123"  # Use short commit hash
```

**Branch-based (for development):**
```cmake
CUSTOM_CACHE_KEY "main-latest"  # Use branch name + latest
```

**When NOT to use CUSTOM_CACHE_KEY:**
- ❌ When you need different cache for different build configurations
- ❌ When experimenting with patches/forks (hash prevents conflicts)
- ✅ **For stable dependencies: Always use it**

**Best practice recommendation:**
- ✅ Use semantic version as key for released dependencies: `CUSTOM_CACHE_KEY "12.1.0"`
- ✅ Matches the Git tag for clarity
- ✅ Enables efficient caching across all projects

**Real-world scenario:**
Without `CUSTOM_CACHE_KEY`, if you have 5 projects all using fmt 12.1.0 but with different `OPTIONS`, you'd have 5 copies in `.cpm-cache/`. With `CUSTOM_CACHE_KEY "12.1.0"`, you have just 1 copy.

---

## 🧪 Unit Testing

This project includes comprehensive unit tests for all components using Google Test.

### Test Structure
Each component has its tests in a `unit_tests/` directory:
- `hal/spi/unit_tests/spi_test.cpp` - Tests for SPI component
- `hal/crypto/unit_tests/crypto_test.cpp` - Tests for Crypto component
- `upper_layer/osal/unit_tests/osal_test.cpp` - Tests for OSAL component

### Running Tests
```bash
# Configure with tests enabled
cmake -B build -DBUILD_TESTING=ON

# Build everything including tests
cmake --build build

# Run all tests
cd build && ctest --output-on-failure

# Or run tests individually
./build/hal/spi/spi_test
./build/hal/crypto/crypto_test
./build/upper_layer/osal/osal_test
```

### Test Output
```
Test project /home/danext/devel/cmake1/cpm-recursive-test/build
    Start  1: SpiTest.GetInfoReturnsCorrectString
1/10 Test  #1: SpiTest.GetInfoReturnsCorrectString ...   Passed    0.00 sec
    Start  2: SpiTest.FormatMessageReturnsFormattedString
2/10 Test  #2: SpiTest.FormatMessageReturnsFormattedString   Passed    0.00 sec
    ...
100% tests passed, 0 tests failed out of 10
```

## 📊 Expected Output

```
=== CPM Recursive Dependencies Test ===

Osal Info:
osal - OS Abstraction Layer
  |-- fmt 12.1.0
  +-- crypto - Cryptography HAL Component
    +-- spi - SPI HAL Component
      |-- fmt 12.1.0
      +-- nlohmann/json 3.11.3


Executing command through dependency chain:
[osal] Final result: [crypto] Processed: [spi] Hello from C++23!

[OK] All recursive dependencies working correctly!
```

### Why add_subdirectory() for local components?
**Lesson learned**: We initially tried using CPM with `SOURCE_DIR` for local components, but discovered issues:
- CPM's `SOURCE_CACHE` + `SOURCE_DIR` combination causes caching problems
- When CPMDeclarePackage is called from add_subdirectory context, SOURCE_DIR gets lost
- Local components are not "dependencies" - they're part of your source tree

**Solution**: Use standard CMake `add_subdirectory()` for local components:
- Simpler and more reliable
- No caching complications
- Standard CMake practice
- CPM is designed for external dependencies, not local project structure

### Why CPM for external dependencies?
- Automatic downloading and version management
- Source caching for faster rebuilds
- Declarative dependency management with CPMDeclarePackage
- Works offline after first build

---

## 🩹 Patching Dependencies

CPM supports applying patches to downloaded dependencies using the `PATCHES` parameter. This is useful for:
- **Bug fixes** - Apply upstream fixes before they're released
- **Custom modifications** - Adapt libraries to your specific needs
- **Compatibility fixes** - Resolve build issues for your platform
- **Feature backports** - Add features to older versions

### Example: Patching nlohmann/json

This project demonstrates patching with two simple patches to nlohmann/json:

**1. Create patch files:**

`hal/patches/nlohmann_json/01-add-custom-comment.patch`:
```patch
--- a/README.md
+++ b/README.md
@@ -25,6 +25,8 @@
 [![Discord](https://img.shields.io/discord/1003743314341793913)](https://discord.gg/6mrGXKvX7y)
 
 - [Design goals](#design-goals)
+
+> 🔧 **CPM Patch Demo**: This library has been patched to demonstrate CPM's PATCHES feature
 - [Sponsors](#sponsors)
 - [Support](#support)
```

`hal/patches/nlohmann_json/02-customize-namespace.patch`:
```patch
--- a/CMakeLists.txt
+++ b/CMakeLists.txt
@@ -1,3 +1,5 @@
+## PATCHED VERSION - CPM Demonstration
+
 cmake_minimum_required(VERSION 3.1...3.14)
```

**2. Apply patches in CPMDeclarePackage:**

```cmake
# hal/CMakeLists.txt
CPMDeclarePackage(
  nlohmann_json
  NAME nlohmann_json
  VERSION 3.11.3
  GITHUB_REPOSITORY nlohmann/json
  GIT_TAG v3.11.3
  CUSTOM_CACHE_KEY "3.11.3-patched"  # ← Different key for patched version
  PATCHES
    "${CMAKE_CURRENT_LIST_DIR}/patches/nlohmann_json/01-add-custom-comment.patch"
    "${CMAKE_CURRENT_LIST_DIR}/patches/nlohmann_json/02-customize-namespace.patch"
  SYSTEM YES
  GIT_SHALLOW YES
  EXCLUDE_FROM_ALL YES
)
```

### How It Works

1. **Download** - CPM clones the repository at specified GIT_TAG
2. **Apply patches** - Patches are applied using `git apply` or `patch` command
3. **Build** - The patched source is built as usual
4. **Cache** - Patched version is cached separately (note the `-patched` suffix in CUSTOM_CACHE_KEY)

### Creating Patches

**From Git diff:**
```bash
# Make changes to downloaded dependency
cd .cpm-cache/nlohmann_json/3.11.3/
# Edit files...
git diff > ../../patches/nlohmann_json/my-fix.patch
```

**Standard patch format:**
```bash
diff -u original.cpp modified.cpp > my-fix.patch
```

### Best Practices
✅ **DO:**
- Use CUSTOM_CACHE_KEY with `-patched` suffix to separate patched versions
- Store patches in project-specific directory (e.g., `hal/patches/<package-name>/`)
- Name patches descriptively: `01-fix-bug.patch`, `02-add-feature.patch`
- Document why each patch is needed (comments in CMakeLists.txt)
- Keep patches minimal and focused
- Test that patches apply cleanly after dependency updates

❌ **DON'T:**
- Use patches for major changes (consider forking instead)
- Forget to regenerate lock file when adding/removing patches (see [Updating Dependencies](#updating-dependencies))
- Apply patches that break the dependency's API

### Workflow: Adding/Removing Patchess

**Important:** When you add or remove patches, you must clear the cached dependency and regenerate the lock file.

**Why?** CPM caches dependencies based on `CUSTOM_CACHE_KEY`. If the cached version already exists, CPM won't re-download and won't apply your new patches.

**Workflow when adding/removing patches:**

```bash
# 1. Edit your CMakeLists.txt to add/remove PATCHES parameter

# 2. Clear the cached dependency
rm -rf .cpm-cache/nlohmann_json

# 3. Clear and regenerate lock file
rm package-lock.cmake
cmake -B build
cmake --build build --target cpm-update-package-lock

# 4. Rebuild
cmake --build build
```

**See [Updating Dependencies](#updating-dependencies) for detailed lock file update workflow.**

**Note:** If you **don't use** `CUSTOM_CACHE_KEY` at all, CPM automatically generates a unique cache key that includes patches. In that case, changing patches creates a different cache location automatically, and you only need to regenerate the lock file.

### Verification

Check if patches were applied:
```bash
# Look for patched content in cached source
grep "CPM Patch Demo" .cpm-cache/nlohmann_json/3.11.3-patched/README.md
head -3 .cpm-cache/nlohmann_json/3.11.3-patched/CMakeLists.txt
```

### When Patches Fail

If configuration fails with patch errors:
```
patching file CMakeLists.txt
Hunk #1 FAILED at 1.
```

**Solutions:**
1. Check patch file format (unified diff with correct line numbers)
2. Verify patch matches the exact GIT_TAG version
3. Test patch manually: `cd .cpm-cache/<pkg> && patch -p1 < patch-file.patch`
4. Update patch if dependency version changed

---

## 🔧 Configuration Options

### CPM Source Cache
```bash
# Local cache (default)
cmake -B build -DCPM_SOURCE_CACHE="${PWD}/.cpm-cache"

# Global cache
cmake -B build -DCPM_SOURCE_CACHE="$HOME/.cache/CPM"

# Disable cache (always download fresh)
cmake -B build -DCPM_SOURCE_CACHE=""
```

### Build Options
```bash
# Debug build
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Release build
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Verbose output
cmake -B build -DCMAKE_VERBOSE_MAKEFILE=ON

# Colored diagnostics
cmake -B build -DCMAKE_COLOR_DIAGNOSTICS=ON
```

## 📚 Learn More

- [CPM.cmake Documentation](https://github.com/cpm-cmake/CPM.cmake)
- [fmt Library](https://github.com/fmtlib/fmt)
- [nlohmann/json](https://github.com/nlohmann/json)
- [Modern CMake](https://cliutils.gitlab.io/modern-cmake/)

## 📝 License

MIT License - See [LICENSE](LICENSE) file for details.

## 🤝 Contributing

This is a demonstration project showing modern C++23 project organization with layered architecture and CMake best practices. Feel free to use it as a template!
