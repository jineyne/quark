@echo off
goto:Main

:: Functions

:OpenQHT
    SETLOCAL ENABLEDELAYEDEXPANSION
        ECHO [%*] Start Parsing
        "../../../cmake-build-debug/bin/quark-header-tool.exe" D:\Projects\Quark\Source\Foundation\Core\ D:\Projects\Quark\Source\Foundation\Core\%* ^
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

rem call:OpenQHT CoreApplication.h

:: Component

rem call:OpenQHT Component\CameraComponent.h
rem call:OpenQHT Component\LightComponent.h
rem call:OpenQHT Component\MeshRendererComponent.h

:: Image

rem call:OpenQHT Image\Texture.h

:: Importer

rem call:OpenQHT Importer\Importer.h

:: Managers

rem call:OpenQHT Manager\

:: Material

call:OpenQHT Material\Shader.h

:: Mesh

rem call:OpenQHT Mesh\Mesh.h

:: RenderAPI

rem call:OpenQHT RenderAPI\RenderAPI.h
call:OpenQHT RenderAPI\SamplerState.h

:: Renderer

rem call:OpenQHT Renderer\ParamBlocks.h
rem call:OpenQHT Renderer\Renderer.h

:: Renderer

rem call:OpenQHT Resource\Resource.h
rem call:OpenQHT Resource\ResourceHandle.h
rem call:OpenQHT Resource\Resources.h

:: Scene

rem call:OpenQHT Scene\Actor.h
rem call:OpenQHT Scene\Component.h
rem call:OpenQHT Scene\SceneObject.h
rem call:OpenQHT Scene\Scene.h
rem call:OpenQHT Scene\Transform.h