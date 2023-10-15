@echo off
goto:Main

:: Functions

:OpenQHT
    SETLOCAL ENABLEDELAYEDEXPANSION
        ECHO [%*] Start Parsing
        "../../../cmake-build-debug/bin/quark-header-tool.exe" D:\Projects\Quark\Source\Foundation\Utility\ ^
            D:\Projects\Quark\Source\Foundation\Utility\%* ^
            D:\Projects\Quark\cmake-build-debug\Generated\Utility ^
            --absolute ^
            --package Foundation ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Engine ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Core ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Utility
        ECHO [%*] End Parsing
    ENDLOCAL
EXIT/B 0

:Main

:: Exception

call:OpenQHT Exception\CrashHandler.h

:: FileSystem

call:OpenQHT FileSystem\Path.h

:: Image

call:OpenQHT Image\Color.h

:: Math

call:OpenQHT Math\Degree.h
call:OpenQHT Math\Quaternion.h
call:OpenQHT Math\Radian.h
call:OpenQHT Math\Rect.h
call:OpenQHT Math\Size.h
call:OpenQHT Math\Vector2.h
call:OpenQHT Math\Vector3.h
call:OpenQHT Math\Vector4.h

:: Misc

call:OpenQHT Misc\Time.h
call:OpenQHT Misc\UUID.h

:: Plugin

call:OpenQHT Plugin\DynLibManager.h
call:OpenQHT Plugin\PluginManager.h