project(quark-test-dynlib)

file(GLOB generated_sources Generated/*.g.h Generated/*.g.cpp)

add_library(${PROJECT_NAME} SHARED Sources/MyDynLibTestModule.h Sources/MyDynLibTestModule.cpp Sources/Plugin.cpp ${generated_sources})
target_link_libraries(${PROJECT_NAME} PUBLIC quark quark-test-common)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        $<$<CONFIG:Debug>:DEBUG>)

target_include_directories(${PROJECT_NAME}
        PUBLIC .
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Generated>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sources>")