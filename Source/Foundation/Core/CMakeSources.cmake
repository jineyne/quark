set (CORE_RENDERAPI_INC
        "Core/RenderAPI/CommandBuffer.h"
        "Core/RenderAPI/CommandBufferManager.h"
        "Core/RenderAPI/EventQuery.h"
        "Core/RenderAPI/RenderAPI.h"
        "Core/RenderAPI/RenderAPIFactory.h"
        "Core/RenderAPI/RenderAPIManager.h"
        "Core/RenderAPI/RenderAPITypes.h"
        "Core/RenderAPI/RenderTarget.h"
        "Core/RenderAPI/RenderWindow.h"
        "Core/RenderAPI/RenderWindowManager.h"
        "Core/RenderAPI/VideoModeInfo.h")
set (CORE_RENDERAPI_SRC
        "Core/RenderAPI/CommandBuffer.cpp"
        "Core/RenderAPI/CommandBufferManager.cpp"
        "Core/RenderAPI/EventQuery.cpp"
        "Core/RenderAPI/RenderAPI.cpp"
        "Core/RenderAPI/RenderAPIManager.cpp"
        "Core/RenderAPI/RenderTarget.cpp"
        "Core/RenderAPI/RenderWindow.cpp"
        "Core/RenderAPI/RenderWindowManager.cpp"
        "Core/RenderAPI/VideoModeInfo.cpp")

set (CORE_NOFILTER_INC
        "Core/CoreApplication.h"
        "Core/CorePrerequisites.h")
set (CORE_NOFILTER_SRC
        "Core/CoreApplication.cpp")

set (CORE_SRCS
        ${CORE_RENDERAPI_INC} ${CORE_RENDERAPI_SRC}
        ${CORE_NOFILTER_INC} ${CORE_NOFILTER_SRC})