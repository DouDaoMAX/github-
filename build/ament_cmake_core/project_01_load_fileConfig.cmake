# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_project_01_load_file_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED project_01_load_file_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(project_01_load_file_FOUND FALSE)
  elseif(NOT project_01_load_file_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(project_01_load_file_FOUND FALSE)
  endif()
  return()
endif()
set(_project_01_load_file_CONFIG_INCLUDED TRUE)

# output package information
if(NOT project_01_load_file_FIND_QUIETLY)
  message(STATUS "Found project_01_load_file: 0.0.0 (${project_01_load_file_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'project_01_load_file' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${project_01_load_file_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(project_01_load_file_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "ament_cmake_export_include_directories-extras.cmake;ament_cmake_export_libraries-extras.cmake;ament_cmake_export_dependencies-extras.cmake")
foreach(_extra ${_extras})
  include("${project_01_load_file_DIR}/${_extra}")
endforeach()
