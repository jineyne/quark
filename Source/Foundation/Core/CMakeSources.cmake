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

set (CORE_MANAGER_INC
        "Core/Manager/BufferManager.h"
        "Core/Manager/CommandBufferManager.h"
        "Core/Manager/GpuProgramManager.h"
        "Core/Manager/MeshManager.h"
        "Core/Manager/RenderAPIManager.h"
        "Core/Manager/RenderStateManager.h"
        "Core/Manager/RenderWindowManager.h"
        "Core/Manager/TextureManager.h")
set (CORE_MANAGER_SRC
        "Core/Manager/BufferManager.cpp"
        "Core/Manager/CommandBufferManager.cpp"
        "Core/Manager/GpuProgramManager.cpp"
        "Core/Manager/MeshManager.cpp"
        "Core/Manager/RenderAPIManager.cpp"
        "Core/Manager/RenderStateManager.cpp"
        "Core/Manager/RenderWindowManager.cpp"
        "Core/Manager/TextureManager.cpp")

set (CORE_MESH_INC
        "Core/Mesh/Mesh.h"
        "Core/Mesh/MeshData.h"
        "Core/Mesh/MeshType.h")
set (CORE_MESH_SRC
        "Core/Mesh/Mesh.cpp"
        "Core/Mesh/MeshData.cpp")

set (CORE_RENDERAPI_INC
        "Core/RenderAPI/Buffer.h"
        "Core/RenderAPI/CommandBuffer.h"
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
        "Core/RenderAPI/VideoModeInfo.h")
set (CORE_RENDERAPI_SRC
        "Core/RenderAPI/Buffer.cpp"
        "Core/RenderAPI/CommandBuffer.cpp"
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
        "Core/RenderAPI/VideoModeInfo.cpp")

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

set (CORE_NOFILTER_INC
        "Core/CoreApplication.h"
        "Core/CorePrerequisites.h")
set (CORE_NOFILTER_SRC
        "Core/CoreApplication.cpp")

set (CORE_SRCS
        ${CORE_IMAGE_INC} ${CORE_IMAGE_SRC}
        ${CORE_MANAGER_INC} ${CORE_MANAGER_SRC}
        ${CORE_MESH_INC} ${CORE_MESH_SRC}
        ${CORE_RENDERAPI_INC} ${CORE_RENDERAPI_SRC}
        ${CORE_RESOURCE_INC} ${CORE_RESOURCE_SRC}
        ${CORE_NOFILTER_INC} ${CORE_NOFILTER_SRC})