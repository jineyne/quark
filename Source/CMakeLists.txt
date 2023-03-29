set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")
include (CMake/FindClang.cmake)

set (QUARK_VERSION_MAJOR 0)
set (QUARK_VERSION_MINOR 1)
set (QUARK_VERSION_PATCH 0)

add_subdirectory(Foundation)
add_subdirectory(Tool)

add_subdirectory(Plugin/DX11)
add_subdirectory(Plugin/AssimpImporter)
add_subdirectory(Plugin/FreeImageImporter)