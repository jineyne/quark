@echo off
goto:Main

:: Functions

:OpenQHT
    SETLOCAL ENABLEDELAYEDEXPANSION
        ECHO [%*] Start Parsing
        "../../../cmake-build-debug/bin/quark-header-tool.exe" D:\Projects\Quark\Source\Plugin\AssimpImporter\ ^
            D:\Projects\Quark\Source\Plugin\AssimpImporter\%* ^
            --absolute ^
            --package AssimpImporter ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Engine ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Core ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Utility
        ECHO [%*] End Parsing
    ENDLOCAL
EXIT/B 0

:Main
