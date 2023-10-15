project(quark-rpg)

include(Source/CMakeSources.cmake)

set (PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR})
configure_file("CMake/ProjectConfig.h.in" "${CMAKE_BINARY_DIR}/Generated/RPG/RPGConfig.h")

file(GLOB generated_sources "${PROJECT_GENERATED_DIR}/RPG/*.g.h" "${PROJECT_GENERATED_DIR}/RPG/*.g.cpp")

add_executable(${PROJECT_NAME} ${APP_SRCS} ${generated_sources})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC quark quark-dx11 quark-box2d quark-assimp-importer quark-freeimg-importer quark-freetype-importer)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Generated>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/Generated/RPG>")
target_compile_definitions(${PROJECT_NAME}
        PUBLIC USE_SYSTEM_THROW)