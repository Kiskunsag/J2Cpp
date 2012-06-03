# main executable (v 0.0)
add_executable(j2cpp main.cpp)
set (j2cpp_VERSION_MAJOR 0)
set (j2cpp_VERSION_MINOR 0)

configure_file(
 "${PROJECT_BINARY_DIR}/src/Version.h.in"
 "${PROJECT_BINARY_DIR}/src/Version.h"
 )

include_directories("$(PROJECT_BINARY_DIR)")
