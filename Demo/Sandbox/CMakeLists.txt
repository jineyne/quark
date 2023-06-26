project(quark-sandbox)

include(Source/CMakeSources.cmake)

set (PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR})
configure_file("CMake/ProjectConfig.h.in" "${CMAKE_BINARY_DIR}/Generated/Sandbox/SandboxConfig.h")

file(GLOB generated_sources "${CMAKE_BINARY_DIR}/Generated/Sandbox/*.g.h" "${CMAKE_BINARY_DIR}/Generated/Sandbox/*.g.cpp")

add_executable(${PROJECT_NAME} ${APP_SRCS} ${generated_sources})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC quark quark-dx11 quark-assimp-importer quark-freeimg-importer quark-freetype-importer)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Generated>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/Generated/Sandbox>")
target_compile_definitions(${PROJECT_NAME}
        PUBLIC USE_SYSTEM_THROW)