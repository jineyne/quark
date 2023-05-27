#pragma once

#include "Prerequisites.h"

#undef BOOL
#undef TRUE
#undef FALSE

#define NOMINMAX
#include <DXGI.h>
#include <D3Dcommon.h>
#include <D3D11.h>

#include <D3D11SDKLayers.h>
#include <D3DCompiler.h>

#define DX11_EXPORT PLUGIN_EXPORT

#define SAFE_RELEASE(o) if (o != nullptr) { o->Release(); o = nullptr; }

DECLARE_LOG_CATEGORY_EXTERN(LogDX11, Fatal);

#if defined(DEBUG) | defined(_DEBUG)
#   define HR(X) {                                                                              \
    HRESULT hr = (X);                                                                           \
    if (FAILED(hr)) {                                                                           \
        LPWSTR output;                                                                          \
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |              \
                      FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, hr,                                 \
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &output, 0, NULL);    \
        EXCEPT(FLogDX11, RenderAPIException, output);                                           \
    }                                                                                           \
}
#else
#   define HR(X) (X)
#endif


class DX11BufferManager;
class DX11CommandBufferManager;
class FDX11GpuParamManager;
class DX11InputLayoutManager;
class DX11RenderStateManager;
class DX11RenderWindowManager;

class DX11CommandBuffer;
class DX11Device;
class DX11Driver;
class DX11DriverList;
class DX11EventQuery;
class DX11RenderAPI;
class DX11RenderWindow;
class DX11VideoMode;
class DX11VideoOutputInfo;
class DX11VideoModeInfo;