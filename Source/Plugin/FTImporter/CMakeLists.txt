project (quark-freetype-importer)

include (CMakeSources.cmake)
find_package(Freetype REQUIRED)

file(GLOB generated_sources Generated/*.g.h Generated/*.g.cpp)
add_library(${PROJECT_NAME} SHARED ${PLUGIN_SRCS} ${generated_sources})

target_link_libraries (${PROJECT_NAME} PRIVATE quark Freetype::Freetype)
target_include_directories(${PROJECT_NAME} PRIVATE .)
