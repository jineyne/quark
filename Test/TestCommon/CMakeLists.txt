project(quark-test-common)

file(GLOB generated_sources Generated/*.g.h Generated/*.g.cpp)

add_library(${PROJECT_NAME} SHARED Sources/DynLibTestModule.h Sources/DynLibTestModule.cpp Sources/TestCommonPrerequisites.h ${generated_sources})
target_link_libraries(${PROJECT_NAME} PUBLIC quark)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        $<$<CONFIG:Debug>:DEBUG>
        PRIVATE
        -DTestCommon_EXPORTS)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Generated>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sources>")