project(quark-test)

find_package(GTest CONFIG REQUIRED)

include(Utility/CMakeSources.cmake)

add_executable(${PROJECT_NAME} main.cpp TestUtility.h TestUtility.cpp ${CORE_TESTS} ${UTILITY_TESTS})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC quark GTest::gmock_main GTest::gtest_main)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")