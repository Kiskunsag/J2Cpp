# the name of the project is "JavaToCPP"
project(JavaToCPP)
cmake_minimum_required(VERSION 2.8)

#set compiler flags
set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_C_COMPILER "gcc")

#set directories
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(source)