project(quark-test)

add_subdirectory(TestCommon)
add_subdirectory(TestDynLib)

find_package(GTest CONFIG REQUIRED)

include(Core/CMakeSources.cmake)
include(Utility/CMakeSources.cmake)

file(GLOB generated_sources "${CMAKE_BINARY_DIR}/Generated/Test/*.g.h" "${CMAKE_BINARY_DIR}/Generated/Test/*.g.cpp")

add_executable(${PROJECT_NAME} main.cpp ${CORE_TESTS} ${UTILITY_TESTS} ${generated_sources})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC quark GTest::gmock_main GTest::gtest_main PRIVATE quark-test-common quark-test-dynlib quark-dx11 quark-assimp-importer dxgi d3d11 dxguid d3dcompiler)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Generated>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Utility>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/Generated/Test>")
target_compile_definitions(${PROJECT_NAME}
        PUBLIC USE_SYSTEM_THROW)