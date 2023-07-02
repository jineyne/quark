@echo off
goto:Main

:: Functions

:OpenQHT
    SETLOCAL ENABLEDELAYEDEXPANSION
        ECHO [%*] Start Parsing
        "../../cmake-build-debug/bin/quark-header-tool.exe" D:\Projects\Quark\Test\ D:\Projects\Quark\Test\Utility\%* ^
            --absolute ^
            --package Test ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Engine ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Core ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Utility ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Test ^
            -I D:\Projects\Quark\Test\Utility\
        ECHO [%*] End Parsing
    ENDLOCAL
EXIT/B 0

:Main

:: Serialization

call:OpenQHT Serialization\ArchiveTest.h
rem call:OpenQHT Serialization\BaseClass.h
rem call:OpenQHT Serialization\OtherData.h
rem call:OpenQHT Serialization\SaveData.h