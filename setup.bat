echo off

set path=%PATH%;%~dp0\Development\Tools\GnuWin32\bin;

ECHO [SETUP.BAT] CLONE VCPKG
git clone "https://github.com/Microsoft/vcpkg.git"

ECHO [SETUP.BAT] BUILD VCPKG
call "./vcpkg/bootstrap-vcpkg.bat"

ECHO [SETUP.BAT] INSTALL LIBRARIES
"./vcpkg/vcpkg" install llvm:x64-windows
"./vcpkg/vcpkg" install inja:x64-windows
"./vcpkg/vcpkg" install freetype:x64-windows
"./vcpkg/vcpkg" install freeimage:x64-windows
"./vcpkg/vcpkg" install openal-soft:x64-windows
"./vcpkg/vcpkg" install assimp:x64-windows
"./vcpkg/vcpkg" install yaml-cpp:x64-windows
"./vcpkg/vcpkg" install gtest:x64-windows


"./vcpkg/vcpkg" integrate install

