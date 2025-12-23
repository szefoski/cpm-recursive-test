# CPM Recursive Dependencies Test Framework

A modern C++23 framework demonstrating CPM (CMake Package Manager) recursive dependency management with layered architecture.

## 📋 Overview

This project showcases how to organize a C++23 project with:
- **Layered architecture**: HAL → Upper Layer
- **Local component management** using `add_subdirectory()` for project components
- **CPM for external dependencies** (fmt, nlohmann/json)
- **Modern CMake** practices with target-based linking
- **C++23** features including `std::println`
- **CPM v0.42.0** vendored for reproducibility
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
- **External Dependencies**: Managed centrally via CPM
- **Local Components**: Organized by layer with add_subdirectory()

## 🛠️ Technologies

- **C++23** - Latest C++ standard with `std::println`
- **CMake 3.23+** - Modern build system
- **CPM.cmake v0.42.0** - External dependency management
- **fmt 12.1.0** - Fast formatting library
- **nlohmann/json 3.11.3** - JSON library

## 📁 Project Structure

```
cpm-recursive-test/
├── CMakeLists.txt              # Root configuration
├── cmake/
│   └── CPM.cmake              # Vendored CPM v0.42.0
├── src/
│   ├── CMakeLists.txt         # Main executable
│   └── main.cpp               # Entry point
├── hal/                       # Hardware Abstraction Layer
│   ├── CMakeLists.txt
│   ├── spi/                   # SPI HAL component
│   │   ├── CMakeLists.txt
│   │   ├── include/spi.hpp
│   │   └── src/spi.cpp
│   └── crypto/                # Crypto HAL component
│       ├── CMakeLists.txt
│       ├── include/crypto.hpp
│       └── src/crypto.cpp
├── upper_layer/               # Upper Layer
│   ├── CMakeLists.txt
│   └── osal/                  # OS Abstraction Layer
│       ├── CMakeLists.txt
│       ├── include/osal.hpp
│       └── src/osal.cpp
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
  - Declared centrally in root CMakeLists.txt
  - Cached in `.cpm-cache/` directory
- **Local components**: Organized with `add_subdirectory()`
  - HAL components in `hal/`
  - Upper layer components in `upper_layer/`
  - Source directory in `src/`

## 🚀 Building

### Prerequisites

- CMake 3.23 or higher
- C++23 compatible compiler (GCC 14+, Clang 17+, MSVC 2022+)
- Internet connection (first build only)

### Build Commands

```bash
# Configure
cmake -B build

# Build
cmake --build build

# Run
./build/src/main
```

### Offline Build

After the first successful build, `.cpm-cache/` contains all external dependencies for offline builds.

## 🎯 CMake Organization

### Root CMakeLists.txt
```cmake
# Declare external dependencies centrally
cpmdeclarepackage(fmt
  NAME fmt
  VERSION 12.1.0
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 12.1.0
  CUSTOM_CACHE_KEY "12.1.0"
  OPTIONS "FMT_INSTALL YES")

# Add component layers
add_subdirectory(hal)
add_subdirectory(upper_layer)
add_subdirectory(src)
```

### Layer CMakeLists.txt
```cmake
# hal/CMakeLists.txt
add_subdirectory(spi)
add_subdirectory(crypto)
```

### Component CMakeLists.txt Pattern
```cmake
# hal/spi/CMakeLists.txt
cmake_minimum_required(VERSION 3.23)
project(spi VERSION 1.0.0 LANGUAGES CXX)

# Request external dependencies by name
cpmaddpackage(NAME fmt)
cpmaddpackage(NAME nlohmann_json)

add_library(spi src/spi.cpp)
target_include_directories(spi PUBLIC ...)
target_link_libraries(spi PRIVATE fmt::fmt nlohmann_json::nlohmann_json)
```

### Key Patterns
- **CPMDeclarePackage** at root: Declare external dependencies once with full parameters
- **CPMAddPackage** in components: Request declared dependencies by NAME only
- **add_subdirectory**: Include local component layers and subdirectories
- **No CPM for local components**: Use standard CMake `add_subdirectory()` for project components

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

## 💡 Design Decisions

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
- Centralized version declarations with CPMDeclarePackage
- Works offline after first build

### Why layered directory structure?
- Clear separation of concerns (HAL vs upper layer)
- Easy to navigate and understand
- Scalable for adding more components
- Matches common embedded/systems architecture patterns
- Each layer has its own CMakeLists.txt for organization

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
