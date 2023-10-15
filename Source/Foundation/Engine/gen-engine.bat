@echo off
goto:Main

:: Functions

:OpenQHT
    SETLOCAL ENABLEDELAYEDEXPANSION
        ECHO [%*] Start Parsing
        "../../../cmake-build-debug/bin/quark-header-tool.exe" D:\Projects\Quark\Source\Foundation\Engine\ ^
            D:\Projects\Quark\Source\Foundation\Engine\%* ^
            D:\Projects\Quark\cmake-build-debug\Generated\Engine ^
            --absolute ^
            --package Foundation ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Engine ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Core ^
            -I D:\Projects\Quark\cmake-build-debug\Generated\Utility
        ECHO [%*] End Parsing
    ENDLOCAL
EXIT/B 0

:Main

:: NoFilter

call:OpenQHT Application.h

:: AI

call:OpenQHT AI\Decorator\AIBlackboardDecoratorNode.h
call:OpenQHT AI\Decorator\AICooldownDecoratorNode.h

call:OpenQHT AI\Execute\AIMoveRandomPositionExecuteNode.h

call:OpenQHT AI\AIBehaviourTree.h
call:OpenQHT AI\AICompositeNode.h
call:OpenQHT AI\AIDecoratorNode.h
call:OpenQHT AI\AIExecuteNode.h
call:OpenQHT AI\AINode.h
call:OpenQHT AI\AISelectorNode.h
call:OpenQHT AI\AISequenceNode.h
call:OpenQHT AI\AITypes.h

:: Component

call:OpenQHT Component\BehaviourTreeComponent.h

:: Debug

call:OpenQHT Debug\DebugDraw.h
