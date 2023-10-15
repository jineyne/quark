@echo off
goto:Main

:: Functions

:OpenQHT
    SETLOCAL ENABLEDELAYEDEXPANSION
        ECHO [%*] Start Parsing
        "../../../out/build/x64-Debug/bin/quark-header-tool.exe" D:\Projects\Quark\Source\Plugin\Dx11\ D:\Projects\Quark\Source\Plugin\Dx11\%* ^
            D:\Projects\Quark\out\build\x64-Debug\Generated\Dx11 ^
            --absolute ^
            --package DX11 ^
            --api DX11 ^
            -I D:\Projects\Quark\out\build\x64-Debug\Generated\Engine ^
            -I D:\Projects\Quark\out\build\x64-Debug\Generated\Core ^
            -I D:\Projects\Quark\out\build\x64-Debug\Generated\Utility ^
            -I D:\Projects\Quark\Source\Plugin\DX11
        ECHO [%*] End Parsing
    ENDLOCAL
EXIT/B 0

:Main

:: Manager

call:OpenQHT Manager

:: Input

call:OpenQHT Input\DX11InputManager.h

:: RenderAPI

call:OpenQHT RenderAPI\DX11RenderAPI.h

:: Image

call:OpenQHT Image\DX11Texture.h
