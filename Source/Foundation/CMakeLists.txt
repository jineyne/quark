project(quark)

include (Core/CMakeSources.cmake)
include (Engine/CMakeSources.cmake)
include (Utility/CMakeSources.cmake)

set(USE_REFLECTION ON CACHE BOOL "Use Reflection System")

find_package(yaml-cpp CONFIG REQUIRED)

file(GLOB generated_sources
        "${PROJECT_GENERATED_DIR}/Core/*.g.h" "${PROJECT_GENERATED_DIR}/Core/*.g.cpp"
        "${PROJECT_GENERATED_DIR}/Utility/*.g.h" "${PROJECT_GENERATED_DIR}/Utility/*.g.cpp"
        "${PROJECT_GENERATED_DIR}/Engine/*.g.h" "${PROJECT_GENERATED_DIR}/Engine/*.g.cpp")

add_library(${PROJECT_NAME} SHARED ${CORE_SRCS} ${ENGINE_SRCS} ${UTILITY_SRCS} ${generated_sources})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Utility>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Engine>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core>"
        "$<BUILD_INTERFACE:${PROJECT_GENERATED_DIR}/Core>"
        "$<BUILD_INTERFACE:${PROJECT_GENERATED_DIR}/Utility>"
        "$<BUILD_INTERFACE:${PROJECT_GENERATED_DIR}/Engine>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/Engine>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/Core>")

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        $<$<CONFIG:Debug>:DEBUG> UNICODE
        -DREFLECTION_ENABLE=USE_REFLECTION
        PRIVATE
        -DEXPORTS)

target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE Rpcrt4)
endif()