@echo off
goto:Main

:: Functions

:OpenQHT
    SETLOCAL ENABLEDELAYEDEXPANSION
        ECHO [%*] Start Parsing
        "../../../out/build/x64-Debug/bin/quark-header-tool.exe" D:\Projects\Quark\Source\Foundation\Engine\ ^
            D:\Projects\Quark\Source\Foundation\Engine\%* ^
            D:\Projects\Quark\Intermediate\Generated\Engine ^
            --absolute ^
            --package Foundation ^
            -I D:\Projects\Quark\Intermediate\Generated\Engine ^
            -I D:\Projects\Quark\Intermediate\Generated\Core ^
            -I D:\Projects\Quark\Intermediate\Generated\Utility
        ECHO [%*] End Parsing
    ENDLOCAL
EXIT/B 0

:Main

:: NoFilter

call:OpenQHT AI\Decorator\AIBlackboardDecoratorNode.h