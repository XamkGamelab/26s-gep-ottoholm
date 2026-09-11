# ROOT CMakeLists.txt

cmake_minimum_required(VERSION 3.20)
project(GEP26S)

# Ensure the runtime executable and library files are outputted to the same directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")

add_subdirectory(vendor EXCLUDE_FROM_ALL)
add_subdirectory(src)
  