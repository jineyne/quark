set (PLUGIN_MATH_INC
        "Math/Matrix4Functions.h")
set (PLUGIN_MATH_SRC
        "Math/Matrix4Functions.cpp")

set (PLUGIN_RENDERAPI_INC
        "RenderAPI/DX11Buffer.h"
        "RenderAPI/DX11BufferManager.h"
        "RenderAPI/DX11CommandBuffer.h"
        "RenderAPI/DX11CommandBufferManager.h"
        "RenderAPI/DX11Device.h"
        "RenderAPI/DX11Driver.h"
        "RenderAPI/DX11DriverList.h"
        "RenderAPI/DX11EventQuery.h"
        "RenderAPI/DX11GpuBuffer.h"
        "RenderAPI/DX11IndexBuffer.h"
        "RenderAPI/DX11RenderAPI.h"
        "RenderAPI/DX11RenderAPIFactory.h"
        "RenderAPI/DX11RenderStateManager.h"
        "RenderAPI/DX11RenderWindow.h"
        "RenderAPI/DX11RenderWindowManager.h"
        "RenderAPI/DX11SamplerState.h"
        "RenderAPI/DX11VertexBuffer.h"
        "RenderAPI/DX11VideoModeInfo.h")
set (PLUGIN_RENDERAPI_SRC
        "RenderAPI/DX11Buffer.cpp"
        "RenderAPI/DX11BufferManager.cpp"
        "RenderAPI/DX11CommandBuffer.cpp"
        "RenderAPI/DX11CommandBufferManager.cpp"
        "RenderAPI/DX11Device.cpp"
        "RenderAPI/DX11Driver.cpp"
        "RenderAPI/DX11DriverList.cpp"
        "RenderAPI/DX11EventQuery.cpp"
        "RenderAPI/DX11GpuBuffer.cpp"
        "RenderAPI/DX11IndexBuffer.cpp"
        "RenderAPI/DX11RenderAPI.cpp"
        "RenderAPI/DX11RenderAPIFactory.cpp"
        "RenderAPI/DX11RenderStateManager.cpp"
        "RenderAPI/DX11RenderWindow.cpp"
        "RenderAPI/DX11RenderWindowManager.cpp"
        "RenderAPI/DX11SamplerState.cpp"
        "RenderAPI/DX11VertexBuffer.cpp"
        "RenderAPI/DX11VideoModeInfo.cpp")

set (PLUGIN_UTILITY_INC
        "Utility/DX11Mapper.h")
set (PLUGIN_UTILITY_SRC
        "Utility/DX11Mapper.cpp")

set (PLUGIN_NOFILTER_INC
        "DX11Prerequisites.h")
set (PLUGIN_NOFILTER_SRC
        "Plugin.cpp")

set (PLUGIN_SRCS
        ${PLUGIN_MATH_INC} ${PLUGIN_MATH_SRC}
        ${PLUGIN_RENDERAPI_INC} ${PLUGIN_RENDERAPI_SRC}
        ${PLUGIN_UTILITY_INC} ${PLUGIN_UTILITY_SRC}
        ${PLUGIN_NOFILTER_INC} ${PLUGIN_NOFILTER_SRC})