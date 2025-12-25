# CPM-dz-patched.cmake

This is a patched version of [CPM.cmake v0.42.0](https://github.com/cpm-cmake/CPM.cmake/releases/tag/v0.42.0) with a fix for package lock file portability when using the `PATCHES` parameter.

## The Issue

When using CPM's `PATCHES` parameter with `CMAKE_CURRENT_SOURCE_DIR` in library projects, the generated `package-lock.cmake` contains **absolute paths** instead of portable variable references.

### Problem Example

**Library declares patches:**
```cmake
# hal/CMakeLists.txt
CPMDeclarePackage(nlohmann_json
  ...
  PATCHES
    "${CMAKE_CURRENT_SOURCE_DIR}/patches/nlohmann_json/01-fix.patch"
)
```

**Generated lock file (BEFORE patch):**
```cmake
# package-lock.cmake
CPMDeclarePackage(nlohmann_json
  ...
  PATCHES
    "/home/user/project/hal/patches/nlohmann_json/01-fix.patch"  # ← Absolute path!
)
```

**Problem:** Lock file is not portable - breaks on different machines or when shared with team members.

### Root Cause

CPM's `cpm_prettify_package_arguments()` function only handles `CMAKE_SOURCE_DIR` replacement for the `SOURCE_DIR` parameter. It doesn't process the `PATCHES` parameter at all, so:

1. CMake expands `${CMAKE_CURRENT_SOURCE_DIR}` to absolute path before CPM sees it
2. CPM writes the absolute path directly to the lock file
3. The path only works on the original developer's machine

## The Fix

This patched version adds `PATCHES` to the list of multi-value arguments and applies the same `CMAKE_SOURCE_DIR` replacement logic used for `SOURCE_DIR`.

### Changes Made

**File:** `CPM-dz-patched.cmake` (lines 1316 and 1336-1350)

1. **Added `PATCHES` to multiValueArgs:**
```cmake
set(multiValueArgs URL OPTIONS DOWNLOAD_COMMAND PATCHES)  # ← Added PATCHES
```

2. **Added path replacement logic:**
```cmake
foreach(singleOption ${CPM_ARGS_${multiArgName}})
  if(${IS_IN_COMMENT})
    string(APPEND PRETTY_OUT_VAR "#")
  endif()
  # Replace CMAKE_SOURCE_DIR in PATCHES paths with variable reference
  if(${multiArgName} STREQUAL "PATCHES")
    string(REPLACE ${CMAKE_SOURCE_DIR} "\${CMAKE_SOURCE_DIR}" singleOption
                   ${singleOption}
    )
  endif()
  string(APPEND PRETTY_OUT_VAR "    \"${singleOption}\"\n")
endforeach()
```

### Result (AFTER patch)

**Generated lock file:**
```cmake
# package-lock.cmake
CPMDeclarePackage(nlohmann_json
  ...
  PATCHES
    "${CMAKE_SOURCE_DIR}/hal/patches/nlohmann_json/01-fix.patch"  # ← Portable!
)
```

✅ Lock file now works on any machine - fully portable across team and CI/CD.

## Workaround (if not using this patch)

If you can't use this patched version, use `CMAKE_SOURCE_DIR` explicitly in your library's CMakeLists.txt:

```cmake
# hal/CMakeLists.txt
CPMDeclarePackage(nlohmann_json
  ...
  PATCHES
    "${CMAKE_SOURCE_DIR}/hal/patches/nlohmann_json/01-fix.patch"  # ← Use CMAKE_SOURCE_DIR
)
```

Then **manually edit** `package-lock.cmake` after regeneration to ensure paths use `${CMAKE_SOURCE_DIR}`.

## Upstream Status

This issue has not been reported to the CPM.cmake project as of December 2024. Consider contributing this fix upstream to help the community.

**Upstream repository:** https://github.com/cpm-cmake/CPM.cmake

## Usage

This patched version is a drop-in replacement for the official CPM.cmake:

```cmake
# CMakeLists.txt
include(cmake/CPM-dz-patched.cmake)  # Instead of cmake/CPM.cmake
```

All CPM features work exactly as documented in the [official documentation](https://github.com/cpm-cmake/CPM.cmake).

## Version

- **Base version:** CPM.cmake v0.42.0
- **Patch date:** December 2024
- **Patch author:** dz (danext)

## License

CPM.cmake is licensed under the MIT License. This patch maintains the same license.
