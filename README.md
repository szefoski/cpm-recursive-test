# CPM Recursive Dependencies Test Framework

A modern C++23 framework demonstrating CPM (CMake Package Manager) recursive dependency management capabilities.

## 📋 Overview

This project showcases how CPM handles complex dependency chains automatically: 
- **Multi-level recursive dependencies** with automatic resolution
- **Modern CMake** practices with target-based linking
- **C++23** features and standards
- **CPM v0.42.0** vendored for reproducibility and offline builds
- **Full offline support** - no internet required after cloning
- **Source caching** via `CPM_SOURCE_CACHE`

## 🔗 Dependency Chain

```
Main Application
 └─► ProjectC (Top-level component)
      ├─► fmt 10.2.1 (formatting library)
      └─► ProjectB (Middleware component)
           └─► ProjectA (Base component)
                ├─► fmt 12.1.0 (upgraded version)
                └─► GoogleTest 1.17.0 (testing framework)
```

**Key Features Demonstrated:**
- Multiple projects requesting different versions of the same dependency (fmt)
- CPM's automatic deduplication of dependencies
- Transitive dependency resolution
- External (GitHub) and local (SOURCE_DIR) dependencies mixed

## 🛠️ Technologies

- **C++23** - Latest C++ standard with `std::print`
- **CMake 3.23+** - Modern build system
- **CPM.cmake v0.42.0** - Dependency management
- **fmt** - Fast formatting library (versions 10.2.1 & 12.1.0)
- **GoogleTest** - Unit testing framework

## 📁 Project Structure

```
cpm-recursive-test/
├── CMakeLists.txt           # Root CMake configuration
├── main.cpp                 # Entry point
├── cmake/
│   └── CPM.cmake           # Vendored CPM v0.42.0 (45K lines)
├── ProjectA/                # Base library (depends on fmt + googletest)
│   ├── CMakeLists.txt
│   ├── include/
│   └── src/
├── ProjectB/                # Middleware library (depends on ProjectA)
│   ├── CMakeLists.txt
│   ├── include/
│   └── src/
├── ProjectC/                # Top-level library (depends on ProjectB + fmt)
│   ├── CMakeLists.txt
│   ├── include/
│   └── src/
└── .cpm-cache/             # Local dependency cache (gitignored)
```

## 🚀 Building

### Prerequisites

- CMake 3.23 or higher
- C++23 compatible compiler (GCC 13+, Clang 16+, MSVC 2022+)
- Internet connection (first build only)

### Build Commands

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run
./build/main
```

### Offline Build

After the first successful build, you can build offline:

```bash
# The .cpm-cache/ directory contains all dependencies
# Simply reconfigure and build as usual
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## 🎯 CPM Features Demonstrated

### 1. Recursive Dependencies
Each subproject independently declares its dependencies using CPM, and the build system automatically resolves the entire dependency tree.

### 2. Source Caching
```cmake
set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cpm-cache")
```
Dependencies are cached locally in `.cpm-cache/` for faster subsequent builds.

### 3. Vendored CPM for Offline Builds
```cmake
include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)
```
CPM.cmake is committed to the repository, ensuring fully reproducible builds without requiring network access. Dependencies are still downloaded on first build, but CPM itself is always available.

### 4. Dependency Deduplication with Version Conflicts
When multiple projects request different versions of the same dependency (like fmt), CPM uses a **"first wins"** strategy - the first version added is used, and subsequent requests for different versions generate warnings. In this project:
- ProjectC adds fmt 10.2.1 first
- ProjectA requests fmt 12.1.0 but gets 10.2.1 with a warning

This demonstrates CPM's version conflict detection. To avoid conflicts, declare dependencies at the root level or use `CPMDeclarePackage()` for consistent versions across all subprojects.

### 5. Mixed Dependency Sources
- **External**: fmt and GoogleTest from GitHub
- **Local**: ProjectA, ProjectB, ProjectC from local directories

## 📊 Expected Output

```
=== CPM Recursive Dependencies Test ===

Component Info:
ProjectC - Top-level Component (uses ProjectB - Middleware Component (uses ProjectA - Base Component))

Full Dependency Chain:
Main Application
 └─► ProjectC (Top-level component)
      ├─► fmt 10.2.1 (formatting library)
      └─► ProjectB (Middleware component)
           └─► ProjectA (Base component)
                ├─► fmt 12.1.0 (upgraded version)
                └─► GoogleTest 1.17.0 (testing framework)

Executing command through dependency chain:
[ProjectC] Final result: [ProjectB] Processed: [ProjectA] Hello from C++23!

✓ All recursive dependencies working correctly!
```

## 🔧 Configuration Options

### CPM Source Cache
Control where dependencies are cached:

```bash
# Local cache (default)
cmake -B build -DCPM_SOURCE_CACHE="${PWD}/.cpm-cache"

# Global cache
cmake -B build -DCPM_SOURCE_CACHE="$HOME/.cache/CPM"

# Disable cache
cmake -B build -DCPM_SOURCE_CACHE=""
```

### Build Options

```bash
# Debug build
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Verbose output
cmake -B build -DCMAKE_VERBOSE_MAKEFILE=ON

# Use specific compiler
cmake -B build -DCMAKE_CXX_COMPILER=g++-13
```

## 📚 Learn More

- [CPM.cmake Documentation](https://github.com/cpm-cmake/CPM.cmake)
- [fmt Library](https://github.com/fmtlib/fmt)
- [GoogleTest](https://github.com/google/googletest)
- [Modern CMake](https://cliutils.gitlab.io/modern-cmake/)

## 📝 License

MIT License - See [LICENSE](LICENSE) file for details.

## 🤝 Contributing

This is a demonstration project. Feel free to use it as a template for your own CPM-based projects!
