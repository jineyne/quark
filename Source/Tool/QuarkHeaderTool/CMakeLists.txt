project(quark-header-tool)
set (CMAKE_CXX_STANDARD 17)

find_package(inja CONFIG REQUIRED)
find_package(LLVM CONFIG REQUIRED)
find_package(Clang REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
add_definitions(${LLVM_DEFINITIONS})


# Find the libraries that correspond to the LLVM components that we wish to use
llvm_map_components_to_libnames(llvm_libs bitreader core frontendopenmp option X86Info X86TargetMCA)
clang_map_components_to_libnames(clang_libs)

include (CMakeSources.cmake)

add_executable(${PROJECT_NAME} main.cpp ${QHT_SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE . ${LLVM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${llvm_libs} ${clang_libs} Version PUBLIC quark PRIVATE pantor::inja)