set (PLUGIN_MATH_INC
        "Math/Matrix4Functions.h")
set (PLUGIN_MATH_SRC
        "Math/Matrix4Functions.cpp")

set (PLUGIN_RENDERAPI_INC
        "RenderAPI/DX11CommandBuffer.h"
        "RenderAPI/DX11CommandBufferManager.h"
        "RenderAPI/DX11Device.h"
        "RenderAPI/DX11Driver.h"
        "RenderAPI/DX11DriverList.h"
        "RenderAPI/DX11EventQuery.h"
        "RenderAPI/DX11RenderAPI.h"
        "RenderAPI/DX11RenderAPIFactory.h"
        "RenderAPI/DX11RenderWindow.h"
        "RenderAPI/DX11RenderWindowManager.h"
        "RenderAPI/DX11VideoModeInfo.h")
set (PLUGIN_RENDERAPI_SRC
        "RenderAPI/DX11CommandBuffer.cpp"
        "RenderAPI/DX11CommandBufferManager.cpp"
        "RenderAPI/DX11Device.cpp"
        "RenderAPI/DX11Driver.cpp"
        "RenderAPI/DX11DriverList.cpp"
        "RenderAPI/DX11EventQuery.cpp"
        "RenderAPI/DX11RenderAPI.cpp"
        "RenderAPI/DX11RenderAPIFactory.cpp"
        "RenderAPI/DX11RenderWindow.cpp"
        "RenderAPI/DX11RenderWindowManager.cpp"
        "RenderAPI/DX11VideoModeInfo.cpp")

set (PLUGIN_NOFILTER_INC
        "DX11Prerequisites.h")
set (PLUGIN_NOFILTER_SRC
        "Plugin.cpp")

set (PLUGIN_SRCS
        ${PLUGIN_MATH_INC} ${PLUGIN_MATH_SRC}
        ${PLUGIN_RENDERAPI_INC} ${PLUGIN_RENDERAPI_SRC}
        ${PLUGIN_NOFILTER_INC} ${PLUGIN_NOFILTER_SRC})