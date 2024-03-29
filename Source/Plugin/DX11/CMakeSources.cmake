set (PLUGIN_HLSL_INC
        "HLSL/HLSLInclude.h"
        "HLSL/HLSLParamParser.h")
set (PLUGIN_HLSL_SRC
        "HLSL/HLSLInclude.cpp"
        "HLSL/HLSLParamParser.cpp")

set (PLUGIN_IMAGE_INC
        "Image/DX11Texture.h")
set (PLUGIN_IMAGE_SRC
        "Image/DX11Texture.cpp")

set (PLUGIN_INPUT_INC
        "Input/DX11InputManager.h"
        "Input/DX11Keyboard.h"
        "Input/DX11Mouse.h")
set (PLUGIN_INPUT_SRC
        "Input/DX11InputManager.cpp"
        "Input/DX11Keyboard.cpp"
        "Input/DX11Mouse.cpp")

set (PLUGIN_MANAGER_INC
        "Manager/DX11BufferManager.h"
        "Manager/DX11CommandBufferManager.h"
        "Manager/DX11GpuProgramManager.h"
        "Manager/DX11InputLayoutManager.h"
        "Manager/DX11RenderStateManager.h"
        "Manager/DX11RenderWindowManager.h"
        "Manager/DX11TextureManager.h")
set (PLUGIN_MANAGER_SRC
        "Manager/DX11BufferManager.cpp"
        "Manager/DX11CommandBufferManager.cpp"
        "Manager/DX11GpuProgramManager.cpp"
        "Manager/DX11InputLayoutManager.cpp"
        "Manager/DX11RenderStateManager.cpp"
        "Manager/DX11RenderWindowManager.cpp"
        "Manager/DX11TextureManager.cpp")

set (PLUGIN_MATH_INC
        "Math/Matrix4Functions.h")
set (PLUGIN_MATH_SRC
        "Math/Matrix4Functions.cpp")

set (PLUGIN_RENDERAPI_INC
        "RenderAPI/DX11Buffer.h"
        "RenderAPI/DX11CommandBuffer.h"
        "RenderAPI/DX11DepthStencilState.h"
        "RenderAPI/DX11Device.h"
        "RenderAPI/DX11Driver.h"
        "RenderAPI/DX11DriverList.h"
        "RenderAPI/DX11EventQuery.h"
        "RenderAPI/DX11GpuBuffer.h"
        "RenderAPI/DX11GpuParamBlockBuffer.h"
        "RenderAPI/DX11GpuProgram.h"
        "RenderAPI/DX11IndexBuffer.h"
        "RenderAPI/DX11RenderAPI.h"
        "RenderAPI/DX11RenderAPIFactory.h"
        "RenderAPI/DX11RenderWindow.h"
        "RenderAPI/DX11SamplerState.h"
        "RenderAPI/DX11TextureView.h"
        "RenderAPI/DX11VertexBuffer.h"
        "RenderAPI/DX11VideoModeInfo.h")
set (PLUGIN_RENDERAPI_SRC
        "RenderAPI/DX11Buffer.cpp"
        "RenderAPI/DX11CommandBuffer.cpp"
        "RenderAPI/DX11DepthStencilState.cpp"
        "RenderAPI/DX11Device.cpp"
        "RenderAPI/DX11Driver.cpp"
        "RenderAPI/DX11DriverList.cpp"
        "RenderAPI/DX11EventQuery.cpp"
        "RenderAPI/DX11GpuBuffer.cpp"
        "RenderAPI/DX11GpuParamBlockBuffer.cpp"
        "RenderAPI/DX11GpuProgram.cpp"
        "RenderAPI/DX11IndexBuffer.cpp"
        "RenderAPI/DX11RenderAPI.cpp"
        "RenderAPI/DX11RenderAPIFactory.cpp"
        "RenderAPI/DX11RenderWindow.cpp"
        "RenderAPI/DX11SamplerState.cpp"
        "RenderAPI/DX11TextureView.cpp"
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
        ${PLUGIN_HLSL_INC} ${PLUGIN_HLSL_SRC}
        ${PLUGIN_IMAGE_INC} ${PLUGIN_IMAGE_SRC}
        ${PLUGIN_INPUT_INC} ${PLUGIN_INPUT_SRC}
        ${PLUGIN_MANAGER_INC} ${PLUGIN_MANAGER_SRC}
        ${PLUGIN_MATH_INC} ${PLUGIN_MATH_SRC}
        ${PLUGIN_RENDERAPI_INC} ${PLUGIN_RENDERAPI_SRC}
        ${PLUGIN_UTILITY_INC} ${PLUGIN_UTILITY_SRC}
        ${PLUGIN_NOFILTER_INC} ${PLUGIN_NOFILTER_SRC})