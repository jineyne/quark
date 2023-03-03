set (CORE_MANAGER_INC
        "Core/Manager/BufferManager.h"
        "Core/Manager/CommandBufferManager.h"
        "Core/Manager/RenderAPIManager.h"
        "Core/Manager/RenderStateManager.h"
        "Core/Manager/RenderWindowManager.h")
set (CORE_MANAGER_SRC
        "Core/Manager/BufferManager.cpp"
        "Core/Manager/CommandBufferManager.cpp"
        "Core/Manager/RenderAPIManager.cpp"
        "Core/Manager/RenderStateManager.cpp"
        "Core/Manager/RenderWindowManager.cpp")

set (CORE_RENDERAPI_INC
        "Core/RenderAPI/Buffer.h"
        "Core/RenderAPI/CommandBuffer.h"

        "Core/RenderAPI/EventQuery.h"
        "Core/RenderAPI/GpuBuffer.h"
        "Core/RenderAPI/IndexBuffer.h"
        "Core/RenderAPI/RenderAPI.h"
        "Core/RenderAPI/RenderAPIFactory.h"
        "Core/RenderAPI/RenderAPITypes.h"
        "Core/RenderAPI/RenderTarget.h"
        "Core/RenderAPI/RenderWindow.h"
        "Core/RenderAPI/SamplerState.h"
        "Core/RenderAPI/VertexBuffer.h"
        "Core/RenderAPI/VideoModeInfo.h")
set (CORE_RENDERAPI_SRC
        "Core/RenderAPI/Buffer.cpp"
        "Core/RenderAPI/CommandBuffer.cpp"
        "Core/RenderAPI/EventQuery.cpp"
        "Core/RenderAPI/GpuBuffer.cpp"
        "Core/RenderAPI/IndexBuffer.cpp"
        "Core/RenderAPI/RenderAPI.cpp"
        "Core/RenderAPI/RenderTarget.cpp"
        "Core/RenderAPI/RenderWindow.cpp"
        "Core/RenderAPI/SamplerState.cpp"
        "Core/RenderAPI/VertexBuffer.cpp"
        "Core/RenderAPI/VideoModeInfo.cpp")

set (CORE_NOFILTER_INC
        "Core/CoreApplication.h"
        "Core/CorePrerequisites.h")
set (CORE_NOFILTER_SRC
        "Core/CoreApplication.cpp")

set (CORE_SRCS
        ${CORE_MANAGER_INC} ${CORE_MANAGER_SRC}
        ${CORE_RENDERAPI_INC} ${CORE_RENDERAPI_SRC}
        ${CORE_NOFILTER_INC} ${CORE_NOFILTER_SRC})