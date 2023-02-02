project(quark-header-tool)
set (CMAKE_CXX_STANDARD 17)

find_package(inja CONFIG REQUIRED)

include (CMakeSources.cmake)

add_executable(${PROJECT_NAME} main.cpp ${QHT_SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} PUBLIC quark PRIVATE pantor::inja)