include (Core/CMakeSources.cmake)
include (Engine/CMakeSources.cmake)
include (Utility/CMakeSources.cmake)

file(GLOB generated_sources Generated/*.g.h Generated/*.g.cpp)

add_library(${PROJECT_NAME} SHARED ${CORE_SRCS} ${ENGINE_SRCS} ${UTILITY_SRCS} ${generated_sources})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Generated>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Utility>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Engine>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core>"
        "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/Generated/Utility>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/Utility>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/Engine>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/Core>")

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        $<$<CONFIG:Debug>:DEBUG>
        PRIVATE
        -DEXPORTS)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE Rpcrt4)
endif()