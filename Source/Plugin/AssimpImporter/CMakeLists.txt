project(quark-assimp-importer)

include(CMakeSources.cmake)
find_package(assimp CONFIG REQUIRED)

file(GLOB generated_sources Generated/*.g.h Generated/*.g.cpp)
add_library(${PROJECT_NAME} SHARED ${PLUGIN_SRCS} ${generated_sources})

target_link_libraries(${PROJECT_NAME}
        PRIVATE quark assimp::assimp)
target_include_directories(${PROJECT_NAME}
        PRIVATE . ${PRIVATE_INCLUDES} "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Generated>")