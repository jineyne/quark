project(quark-sandbox)

include(Source/CMakeSources.cmake)

file(GLOB GENERATED_SRCS Generated/*.g.h Generated/*.g.cpp)

add_executable(${PROJECT_NAME} ${SRCS} ${GENERATED_SRCS})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC quark quark-dx11 quark-assimp-importer quark-freeimg-importer)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Generated>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Utility>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
target_compile_definitions(${PROJECT_NAME}
        PUBLIC USE_SYSTEM_THROW)