project(quark-header-tool)
set (CMAKE_CXX_STANDARD 17)

find_package(inja CONFIG REQUIRED)
find_package(LLVM CONFIG REQUIRED)
find_package(zstd CONFIG REQUIRED)
find_package(Clang REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
add_definitions(${LLVM_DEFINITIONS})

# Find the libraries that correspond to the LLVM components that we wish to use
llvm_map_components_to_libnames(llvm_libs Analysis core irreader bitreader frontendopenmp option X86Info X86TargetMCA windowsdriver support)
clang_map_components_to_libnames(clang_libs)

include (CMakeSources.cmake)

message("llvm_libs: ${llvm_libs}")
message("clang_libs: ${clang_libs}")

add_executable(${PROJECT_NAME} main.cpp ${QHT_SRCS})
target_include_directories(${PROJECT_NAME}
    PRIVATE
        .
        ${LLVM_INCLUDE_DIRS}
        "$<BUILD_INTERFACE:${APP_ROOT_DIR}/Source/Foundation/Utility>"
        "$<BUILD_INTERFACE:${PROJECT_GENERATED_DIR}/Utility>")
target_link_libraries(${PROJECT_NAME} PRIVATE  Version PRIVATE pantor::inja) # ${llvm_libs} ${clang_libs}
