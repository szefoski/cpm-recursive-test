# CPM Recursive Dependencies Test Framework

A modern C++23 framework demonstrating CPM (CMake Package Manager) recursive dependency management with layered architecture.

## 📑 Table of Contents

- [📋 Overview](#-overview)
- [🔗 Dependency Chain](#-dependency-chain)
- [🛠️ Technologies](#️-technologies)
- [📁 Project Structure](#-project-structure)
- [🏗️ Architecture Principles](#️-architecture-principles)
- [🔒 Package Lock Mechanism](#-package-lock-mechanism)
  - [Philosophy: Libraries vs Applications](#philosophy-libraries-vs-applications)
  - [Comparison to Cargo (Rust)](#comparison-to-cargo-rust)
  - [How It Works](#how-it-works)
  - [Updating Dependencies](#updating-dependencies)
  - [Best Practices](#best-practices)
  - [Benefits](#benefits)
- [🚀 Building](#-building)
  - [Prerequisites](#prerequisites)
  - [Build Commands](#build-commands)
  - [First-Time Setup](#first-time-setup)
  - [Offline Build](#offline-build)
- [🎯 CMake Organization](#-cmake-organization)
- [💡 Design Decisions](#-design-decisions)
- [🧪 Unit Testing](#-unit-testing)
- [📊 Expected Output](#-expected-output)
- [🔧 Configuration Options](#-configuration-options)
- [📚 Learn More](#-learn-more)
- [📝 License](#-license)
- [🤝 Contributing](#-contributing)

## 📋 Overview

This project showcases how to organize a C++23 project with:
- **Layered architecture**: HAL → Upper Layer
- **Local component management** using `add_subdirectory()` for project components
- **CPM for external dependencies** (fmt, nlohmann/json)
- **Modern CMake** practices with target-based linking
- **C++23** features including `std::println`
- **CPM v0.42.0** vendored for reproducibility
- **Package lock file** for reproducible builds (following Cargo/npm best practices)
- **Proper namespacing** and naming conventions

## 🔗 Dependency Chain

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

## 🎯 CMake Organization

### Root CMakeLists.txt
```cmake
# Package lock file - reproducible builds
include(cmake/CPM.cmake)
CPMUsePackageLock(package-lock.cmake)

# Add component layers
add_subdirectory(hal)
add_subdirectory(upper_layer)
add_subdirectory(src)
```

### Library CMakeLists.txt (hal/, upper_layer/)
```cmake
# hal/CMakeLists.txt
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
### Component CMakeLists.txt Pattern
```cmake
# hal/spi/CMakeLists.txt
cmake_minimum_required(VERSION 3.23)
project(spi VERSION 1.0.0 LANGUAGES CXX)

# Request external dependencies by name (versions from parent)
CPMAddPackage(NAME fmt)
CPMAddPackage(NAME nlohmann_json)

add_library(spi src/spi.cpp)
target_include_directories(spi PUBLIC include)
target_link_libraries(spi PRIVATE fmt::fmt nlohmann_json::nlohmann_json)
```

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
