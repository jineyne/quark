project (quark-imgui)

include (CMakeSources.cmake)

file(GLOB generated_sources "${CMAKE_BINARY_DIR}/Generated/ImGui/*.g.h" "${CMAKE_BINARY_DIR}/Generated/ImGui/*.g.cpp")
add_library(${PROJECT_NAME} SHARED ${PLUGIN_SRCS} ${generated_sources})

target_link_libraries (${PROJECT_NAME} PRIVATE quark imgui)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Private>"
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>")

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(TARGETS ${PROJECT_NAME}
        EXPORT quark-imgui-config
        DESTINATION lib)

install(EXPORT quark-imgui-config
        NAMESPACE quark::
        FILE quark-imgui-config.cmake
        DESTINATION cmake/quark)

install(
        DIRECTORY Public
        DESTINATION include/imgui
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "Private" EXCLUDE
)