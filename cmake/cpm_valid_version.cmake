# Advanced function to validate package version with support for: - Exact
# versions: "12.1.0" - Version ranges: "12.3.0...12.3.6" (inclusive) -
# Exclusions: "!12.4.0" (must be combined with other rules) Examples:
# cpm_valid_version(spi fmt "12.1.0" "12.2.1" "12.3.0...12.3.6" "!12.3.4")
function(cpm_valid_version COMPONENT_NAME PACKAGE_NAME)
  set(VERSION_RULES ${ARGN})

  cpmgetpackageversion(${PACKAGE_NAME} PACKAGE_VERSION)

  if(NOT PACKAGE_VERSION)
    message(
      FATAL_ERROR
        "[${COMPONENT_NAME}] Could not determine version of ${PACKAGE_NAME}")
  endif()

  # Parse version components
  string(REPLACE "." ";" VERSION_PARTS ${PACKAGE_VERSION})
  list(LENGTH VERSION_PARTS VERSION_PARTS_COUNT)

  if(VERSION_PARTS_COUNT GREATER_EQUAL 1)
    list(GET VERSION_PARTS 0 VERSION_MAJOR)
  else()
    set(VERSION_MAJOR 0)
  endif()

  if(VERSION_PARTS_COUNT GREATER_EQUAL 2)
    list(GET VERSION_PARTS 1 VERSION_MINOR)
  else()
    set(VERSION_MINOR 0)
  endif()

  if(VERSION_PARTS_COUNT GREATER_EQUAL 3)
    list(GET VERSION_PARTS 2 VERSION_PATCH)
  else()
    set(VERSION_PATCH 0)
  endif()

  set(VERSION_APPROVED FALSE)
  set(EXCLUSIONS "")

  # Process rules
  foreach(RULE ${VERSION_RULES})
    # Check for exclusion rule
    if(RULE MATCHES "^!(.+)$")
      list(APPEND EXCLUSIONS ${CMAKE_MATCH_1})
      continue()
    endif()

    # Check for range rule
    if(RULE MATCHES "^([0-9.]+)\\.\\.\\.\([0-9.]+)$")
      set(RANGE_MIN ${CMAKE_MATCH_1})
      set(RANGE_MAX ${CMAKE_MATCH_2})

      if(PACKAGE_VERSION VERSION_GREATER_EQUAL RANGE_MIN
         AND PACKAGE_VERSION VERSION_LESS_EQUAL RANGE_MAX)
        set(VERSION_APPROVED TRUE)
      endif()
      # Check for exact version
    elseif(PACKAGE_VERSION VERSION_EQUAL RULE)
      set(VERSION_APPROVED TRUE)
    endif()
  endforeach()

  # Check exclusions
  foreach(EXCLUDED_VERSION ${EXCLUSIONS})
    if(PACKAGE_VERSION VERSION_EQUAL EXCLUDED_VERSION)
      list(JOIN VERSION_RULES ", " VERSION_RULES_STR)
      message(
        FATAL_ERROR
          "[${COMPONENT_NAME}] ${PACKAGE_NAME} version ${PACKAGE_VERSION} is explicitly excluded\n"
          "  Version rules: ${VERSION_RULES_STR}")
    endif()
  endforeach()

  # Final validation
  if(NOT VERSION_APPROVED)
    list(JOIN VERSION_RULES ", " VERSION_RULES_STR)
    message(
      FATAL_ERROR
        "[${COMPONENT_NAME}] Requires ${PACKAGE_NAME} matching version rules: ${VERSION_RULES_STR}\n"
        "  But got version: ${PACKAGE_VERSION}")
  endif()

  message(
    STATUS
      "[${COMPONENT_NAME}] Using approved ${PACKAGE_NAME} version: ${PACKAGE_VERSION}"
  )
endfunction()
