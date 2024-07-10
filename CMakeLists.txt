cmake_minimum_required(VERSION 3.15)
project(quark)

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message("VCPKG_TARGET_ARCHITECTURE : ${VCPKG_TARGET_ARCHITECTURE}")

set (PROJECT_GENERATED_DIR "${PROJECT_SOURCE_DIR}/Intermediate/Generated")

add_subdirectory(Source)
add_subdirectory(Demo/RPG)
# add_subdirectory(Demo/Sandbox)
# add_subdirectory(Test)