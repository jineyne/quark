project(quark-header-tool)
set (CMAKE_CXX_STANDARD 17)

include (HeaderParse/CMakeSources.cmake)

add_executable(${PROJECT_NAME} main.cpp ${HEADERPARSER_SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} PUBLIC quark)