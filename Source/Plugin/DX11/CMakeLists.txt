project (quark-dx11)

include(CMakeSources.cmake)

file(GLOB generated_sources "${CMAKE_BINARY_DIR}/Generated/Dx11/*.g.h" "${CMAKE_BINARY_DIR}/Generated/Dx11/*.g.cpp")
add_library(${PROJECT_NAME} SHARED ${PLUGIN_SRCS} ${PRIVATE_SRCS} ${generated_sources})

target_link_libraries(${PROJECT_NAME}
        PUBLIC dxgi d3d11 dxguid d3dcompiler
        PRIVATE quark ${PRIVATE_LIBRARIES})
target_include_directories(${PROJECT_NAME}
        PRIVATE . ${PRIVATE_INCLUDES} "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/Generated/Dx11>")