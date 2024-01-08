#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "curlcpp::curlcpp" for configuration "Debug"
set_property(TARGET curlcpp::curlcpp APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(curlcpp::curlcpp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libcurlcpp.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS curlcpp::curlcpp )
list(APPEND _IMPORT_CHECK_FILES_FOR_curlcpp::curlcpp "${_IMPORT_PREFIX}/lib/libcurlcpp.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
