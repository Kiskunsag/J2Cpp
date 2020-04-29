# main executable (v 0.0)
add_executable(j2cpp main.cpp)
set (j2cpp_VERSION_MAJOR 0)
set (j2cpp_VERSION_MINOR 0)

configure_file(
 "${PROJECT_SOURCE_DIR}/src/version.h.in"
 "${PROJECT_BINARY_DIR}/src/version.h"
 )

#ensure that Version.h is easily found
include_directories("$(PROJECT_BINARY_DIR)/src")
