@echo off
goto:Main

:: Functions

:OpenQHT
    SETLOCAL ENABLEDELAYEDEXPANSION
        ECHO [%*] Start Parsing
        "../../cmake-build-debug/bin/quark-header-tool.exe" D:\Projects\quark\Demo\Sandbox\ ^
            D:\Projects\quark\Demo\Sandbox\Source\%* ^
            D:\Projects\Quark\cmake-build-debug\Generated\Sandbox ^
            --absolute ^
            --package Foundation ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Engine ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Core ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Utility
        ECHO [%*] End Parsing
    ENDLOCAL
EXIT/B 0

:Main

:: Component

call:OpenQHT Component\BattleFleetAIComponent.h
call:OpenQHT Component\BulletComponent.h
call:OpenQHT Component\ShipAIComponent.h
call:OpenQHT Component\FighterAIComponent.h
call:OpenQHT Component\FollowTargetComponent.h
call:OpenQHT Component\PlayerInputComponent.h
call:OpenQHT Component\PlayerShipComponent.h
call:OpenQHT Component\ShipAIComponent.h
call:OpenQHT Component\SpawnerComponent.h
