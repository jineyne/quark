set (BOX2D_BUILD_TESTBED OFF)
set (BOX2D_INSTALL OFF)
add_subdirectory(Box2d)

# ImGui
project (imgui)

file (GLOB SRCS imgui/*.cpp)
list (FILTER SRCS EXCLUDE REGEX ".*test.cpp")

add_library (imgui
    STATIC ${SRCS}
        imgui/backends/imgui_impl_dx11.cpp imgui/backends/imgui_impl_dx11.h
        imgui/backends/imgui_impl_win32.cpp imgui/backends/imgui_impl_win32.h)

# test cpp files should be automatically gathered by the unittest library
target_compile_definitions (imgui PUBLIC -DIMGUI_DISABLE_OSX_FUNCTIONS -DIMGUI_DISABLE_WIN32_FUNCTIONS)
target_include_directories (imgui PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>")