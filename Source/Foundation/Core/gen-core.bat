@echo off
goto:Main

:: Functions

:OpenQHT
    SETLOCAL ENABLEDELAYEDEXPANSION
        ECHO [%*] Start Parsing
        "../../../cmake-build-debug/bin/quark-header-tool.exe" D:\Projects\Quark\Source\Foundation\Core\ ^
            D:\Projects\Quark\Source\Foundation\Core\%* ^
            D:\Projects\Quark\cmake-build-debug\Generated\Core ^
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

call:OpenQHT CoreApplication.h

:: Component

call:OpenQHT Component\BoxCollider2DComponent.h
call:OpenQHT Component\CameraComponent.h
call:OpenQHT Component\Collider2DComponent.h
call:OpenQHT Component\LightComponent.h
call:OpenQHT Component\MeshRendererComponent.h
call:OpenQHT Component\SphereCollider2DComponent.h
call:OpenQHT Component\SpriteRendererComponent.h

:: Image

call:OpenQHT Image\PixelUtil.h
call:OpenQHT Image\Texture.h

:: Importer

call:OpenQHT Importer\Importer.h

:: Managers

call:OpenQHT Manager\

:: Material

call:OpenQHT Material\Pass.h
call:OpenQHT Material\Shader.h
call:OpenQHT Material\ShaderVariation.h
call:OpenQHT Material\Technique.h

:: Mesh

call:OpenQHT Mesh\Mesh.h

:: Physics

call:OpenQHT Physics\Physics.h

:: RenderAPI

call:OpenQHT RenderAPI\RenderAPI.h
call:OpenQHT RenderAPI\SamplerState.h

:: Renderer

call:OpenQHT Renderer\ParamBlocks.h
call:OpenQHT Renderer\Renderer.h

:: Renderer

call:OpenQHT Resource\Resource.h
call:OpenQHT Resource\ResourceHandle.h
call:OpenQHT Resource\Resources.h

:: Scene

call:OpenQHT Scene\Actor.h
call:OpenQHT Scene\Component.h
call:OpenQHT Scene\SceneObject.h
call:OpenQHT Scene\Scene.h
call:OpenQHT Scene\Transform.h