set (CORE_COMPONENT_INC
        "Core/Component/CameraComponent.h"
        "Core/Component/CubeColliderComponent.h"
        "Core/Component/LightComponent.h"
        "Core/Component/MeshRendererComponent.h"
        "Core/Component/RigidBodyComponent.h"
        "Core/Component/SphereColliderComponent.h")
set (CORE_COMPONENT_SRC
        "Core/Component/CameraComponent.cpp"
        "Core/Component/CubeColliderComponent.cpp"
        "Core/Component/LightComponent.cpp"
        "Core/Component/MeshRendererComponent.cpp"
        "Core/Component/RigidBodyComponent.cpp"
        "Core/Component/SphereColliderComponent.cpp")

set (CORE_IMAGE_INC
        "Core/Image/PixelData.h"
        "Core/Image/PixelUtil.h"
        "Core/Image/PixelVolume.h"
        "Core/Image/Texture.h"
        "Core/Image/TextureSurface.h")
set (CORE_IMAGE_SRC
        "Core/Image/PixelData.cpp"
        "Core/Image/PixelUtil.cpp"
        "Core/Image/Texture.cpp"
        "Core/Image/TextureSurface.cpp")

set (CORE_IMPORTER_INC
        "Core/Importer/Importer.h"
        "Core/Importer/ImporterOptions.h"
        "Core/Importer/SpecificImporter.h")
set (CORE_IMPORTER_SRC
        "Core/Importer/Importer.cpp"
        "Core/Importer/ImporterOptions.cpp"
        "Core/Importer/SpecificImporter.cpp")

set (CORE_INPUT_INC
        "Core/Input/InputDevice.h"
        "Core/Input/InputType.h")
set (CORE_INPUT_SRC
        "Core/Input/InputDevice.cpp")

set (CORE_MANAGER_INC
        "Core/Manager/BufferManager.h"
        "Core/Manager/CommandBufferManager.h"
        "Core/Manager/GpuProgramManager.h"
        "Core/Manager/InputManager.h"
        "Core/Manager/MeshManager.h"
        "Core/Manager/RenderAPIManager.h"
        "Core/Manager/RenderStateManager.h"
        "Core/Manager/RenderWindowManager.h"
        "Core/Manager/SceneManager.h"
        "Core/Manager/SceneObjectManager.h"
        "Core/Manager/TextureManager.h")
set (CORE_MANAGER_SRC
        "Core/Manager/BufferManager.cpp"
        "Core/Manager/CommandBufferManager.cpp"
        "Core/Manager/GpuProgramManager.cpp"
        "Core/Manager/InputManager.cpp"
        "Core/Manager/MeshManager.cpp"
        "Core/Manager/RenderAPIManager.cpp"
        "Core/Manager/RenderStateManager.cpp"
        "Core/Manager/RenderWindowManager.cpp"
        "Core/Manager/SceneManager.cpp"
        "Core/Manager/SceneObjectManager.cpp"
        "Core/Manager/TextureManager.cpp")

set (CORE_MATERIAL_INC
        "Core/Material/GpuParamsSet.h"
        "Core/Material/Material.h"
        "Core/Material/MaterialParam.h"
        "Core/Material/MaterialParams.h"
        "Core/Material/MaterialType.h"
        "Core/Material/Pass.h"
        "Core/Material/Shader.h"
        "Core/Material/ShaderVariation.h"
        "Core/Material/Technique.h")
set (CORE_MATERIAL_INC
        "Core/Material/GpuParamsSet.cpp"
        "Core/Material/Material.cpp"
        "Core/Material/MaterialParam.cpp"
        "Core/Material/MaterialParams.cpp"
        "Core/Material/Pass.cpp"
        "Core/Material/Shader.cpp"
        "Core/Material/ShaderVariation.cpp"
        "Core/Material/Technique.cpp")

set (CORE_MESH_INC
        "Core/Mesh/Mesh.h"
        "Core/Mesh/MeshData.h"
        "Core/Mesh/MeshType.h")
set (CORE_MESH_SRC
        "Core/Mesh/Mesh.cpp"
        "Core/Mesh/MeshData.cpp")

set (CORE_PHYSICS_INC
        "Core/Physics/AABBTree.h"
        "Core/Physics/Collider.h"
        "Core/Physics/CubeCollider.h"
        "Core/Physics/Physics.h"
        "Core/Physics/PhysicsDebugRenderer.h"
        "Core/Physics/PlaneCollider.h"
        "Core/Physics/RigidBody.h"
        "Core/Physics/SphereCollider.h")
set (CORE_PHYSICS_SRC
        "Core/Physics/AABBTree.cpp"
        "Core/Physics/Collider.cpp"
        "Core/Physics/CubeCollider.cpp"
        "Core/Physics/Physics.cpp"
        "Core/Physics/PhysicsDebugRenderer.cpp"
        "Core/Physics/PlaneCollider.cpp"
        "Core/Physics/RigidBody.cpp"
        "Core/Physics/SphereCollider.cpp")

set (CORE_RENDERAPI_INC
        "Core/RenderAPI/Buffer.h"
        "Core/RenderAPI/CommandBuffer.h"
        "Core/RenderAPI/DepthStencilState.h"
        "Core/RenderAPI/EventQuery.h"
        "Core/RenderAPI/GpuBuffer.h"
        "Core/RenderAPI/GpuParam.h"
        "Core/RenderAPI/GpuParamBlockBuffer.h"
        "Core/RenderAPI/GpuParamDesc.h"
        "Core/RenderAPI/GpuParams.h"
        "Core/RenderAPI/GpuPipelineParamInfo.h"
        "Core/RenderAPI/GpuProgram.h"
        "Core/RenderAPI/GraphicsPipelineState.h"
        "Core/RenderAPI/IndexBuffer.h"
        "Core/RenderAPI/RenderAPI.h"
        "Core/RenderAPI/RenderAPICapabilities.h"
        "Core/RenderAPI/RenderAPIFactory.h"
        "Core/RenderAPI/RenderAPITypes.h"
        "Core/RenderAPI/RenderTarget.h"
        "Core/RenderAPI/RenderWindow.h"
        "Core/RenderAPI/SamplerState.h"
        "Core/RenderAPI/TextureView.h"
        "Core/RenderAPI/VertexBuffer.h"
        "Core/RenderAPI/VertexData.h"
        "Core/RenderAPI/VertexDataDesc.h"
        "Core/RenderAPI/VertexDeclaration.h"
        "Core/RenderAPI/VideoModeInfo.h"
        "Core/RenderAPI/Viewport.h")
set (CORE_RENDERAPI_SRC
        "Core/RenderAPI/Buffer.cpp"
        "Core/RenderAPI/CommandBuffer.cpp"
        "Core/RenderAPI/DepthStencilState.cpp"
        "Core/RenderAPI/EventQuery.cpp"
        "Core/RenderAPI/GpuBuffer.cpp"
        "Core/RenderAPI/GpuParam.cpp"
        "Core/RenderAPI/GpuParamBlockBuffer.cpp"
        "Core/RenderAPI/GpuParamDesc.cpp"
        "Core/RenderAPI/GpuParams.cpp"
        "Core/RenderAPI/GpuPipelineParamInfo.cpp"
        "Core/RenderAPI/GpuProgram.cpp"
        "Core/RenderAPI/GraphicsPipelineState.cpp"
        "Core/RenderAPI/IndexBuffer.cpp"
        "Core/RenderAPI/RenderAPI.cpp"
        "Core/RenderAPI/RenderAPICapabilities.cpp"
        "Core/RenderAPI/RenderTarget.cpp"
        "Core/RenderAPI/RenderWindow.cpp"
        "Core/RenderAPI/SamplerState.cpp"
        "Core/RenderAPI/TextureView.cpp"
        "Core/RenderAPI/VertexBuffer.cpp"
        "Core/RenderAPI/VertexData.cpp"
        "Core/RenderAPI/VertexDataDesc.cpp"
        "Core/RenderAPI/VertexDeclaration.cpp"
        "Core/RenderAPI/VideoModeInfo.cpp"
        "Core/RenderAPI/Viewport.cpp")

set (CORE_RENDERER_INC
        "Core/Renderer/CameraBase.h"
        "Core/Renderer/LightBase.h"
        "Core/Renderer/ParamBlocks.h"
        "Core/Renderer/Renderable.h"
        "Core/Renderer/RenderableInfo.h"
        "Core/Renderer/RenderableLight.h"
        "Core/Renderer/RenderElement.h"
        "Core/Renderer/Renderer.h"
        "Core/Renderer/RendererExtension.h"
        "Core/Renderer/RendererTypes.h"
        "Core/Renderer/RenderQueue.h"
        "Core/Renderer/RenderSettings.h"
        "Core/Renderer/SceneInfo.h"
        "Core/Renderer/ViewInfo.h")
set (CORE_RENDERER_SRC
        "Core/Renderer/CameraBase.cpp"
        "Core/Renderer/LightBase.cpp"
        "Core/Renderer/ParamBlocks.cpp"
        "Core/Renderer/Renderable.cpp"
        "Core/Renderer/RenderableInfo.cpp"
        "Core/Renderer/RenderableLight.cpp"
        "Core/Renderer/RenderElement.cpp"
        "Core/Renderer/Renderer.cpp"
        "Core/Renderer/RendererExtension.cpp"
        "Core/Renderer/RenderQueue.cpp"
        "Core/Renderer/RenderSettings.cpp"
        "Core/Renderer/SceneInfo.cpp"
        "Core/Renderer/ViewInfo.cpp")

set (CORE_RESOURCE_INC
        "Core/Resource/GpuResourceData.h"
        "Core/Resource/Resource.h"
        "Core/Resource/ResourceArchive.h"
        "Core/Resource/ResourceHandle.h"
        "Core/Resource/Resources.h")
set (CORE_RESOURCE_SRC
        "Core/Resource/GpuResourceData.cpp"
        "Core/Resource/Resource.cpp"
        "Core/Resource/ResourceArchive.cpp"
        "Core/Resource/ResourceHandle.cpp"
        "Core/Resource/Resources.cpp")

set (CORE_SCENE_INC
        "Core/Scene/Actor.h"
        "Core/Scene/Component.h"
        "Core/Scene/SceneObject.h"
        "Core/Scene/Scene.h"
        "Core/Scene/SceneTypes.h"
        "Core/Scene/Transform.h")
set (CORE_SCENE_SRC
        "Core/Scene/Actor.cpp"
        "Core/Scene/Component.cpp"
        "Core/Scene/SceneObject.cpp"
        "Core/Scene/Scene.cpp"
        "Core/Scene/SceneTypes.cpp"
        "Core/Scene/Transform.cpp")

set (CORE_NOFILTER_INC
        "Core/CoreApplication.h"
        "Core/CorePrerequisites.h")
set (CORE_NOFILTER_SRC
        "Core/CoreApplication.cpp")

set (CORE_SRCS
        ${CORE_COMPONENT_INC} ${CORE_COMPONENT_SRC}
        ${CORE_IMAGE_INC} ${CORE_IMAGE_SRC}
        ${CORE_IMPORTER_INC} ${CORE_IMPORTER_SRC}
        ${CORE_INPUT_INC} ${CORE_INPUT_SRC}
        ${CORE_MANAGER_INC} ${CORE_MANAGER_SRC}
        ${CORE_MATERIAL_INC} ${CORE_MATERIAL_SRC}
        ${CORE_MESH_INC} ${CORE_MESH_SRC}
        ${CORE_PHYSICS_INC} ${CORE_PHYSICS_SRC}
        ${CORE_RENDERAPI_INC} ${CORE_RENDERAPI_SRC}
        ${CORE_RENDERER_INC} ${CORE_RENDERER_SRC}
        ${CORE_RESOURCE_INC} ${CORE_RESOURCE_SRC}
        ${CORE_SCENE_INC} ${CORE_SCENE_SRC}
        ${CORE_NOFILTER_INC} ${CORE_NOFILTER_SRC})