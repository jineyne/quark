project(quark-test)

add_subdirectory(TestCommon)
add_subdirectory(TestDynLib)

find_package(GTest CONFIG REQUIRED)

include(Utility/CMakeSources.cmake)

file(GLOB generated_sources Generated/*.g.h Generated/*.g.cpp)

add_executable(${PROJECT_NAME} main.cpp ${CORE_TESTS} ${UTILITY_TESTS} ${generated_sources})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC quark GTest::gmock_main GTest::gtest_main PRIVATE quark-test-common quark-test-dynlib)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Generated>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Utility>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
target_compile_definitions(${PROJECT_NAME}
        PUBLIC USE_SYSTEM_THROW)