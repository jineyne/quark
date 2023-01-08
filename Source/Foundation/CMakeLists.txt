include (Core/CMakeSources.cmake)
include (Utility/CMakeSources.cmake)

add_library(${PROJECT_NAME} SHARED ${CORE_SRCS} ${UTILITY_SRCS})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Utility>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core>"
        "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/Generated/Utility>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/Utility>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/Core>")
target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        $<$<CONFIG:Debug>:DEBUG>
        PRIVATE
        -DEXPORTS)