set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")
include (CMake/FindClang.cmake)

set (QUARK_VERSION_MAJOR 0)
set (QUARK_VERSION_MINOR 1)
set (QUARK_VERSION_PATCH 0)

set (APP_ROOT_DIR ${PROJECT_SOURCE_DIR})
set (PROJECT_GENERATED_DIR "${PROJECT_BINARY_DIR}/Generated")

add_subdirectory(Foundation)
add_subdirectory(Tool)

add_subdirectory(Plugin/DX11)
add_subdirectory(Plugin/AssimpImporter)
add_subdirectory(Plugin/FreeImageImporter)
add_subdirectory(Plugin/FTImporter)

configure_file("CMake/FrameworkConfig.h.in" "${CMAKE_BINARY_DIR}/Generated/Utility/FrameworkConfig.h")
configure_file("CMake/EngineConfig.h.in" "${CMAKE_BINARY_DIR}/Generated/Engine/EngineConfig.h")