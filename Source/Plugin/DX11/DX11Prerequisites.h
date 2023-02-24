#pragma once

#include "Prerequisites.h"

#define NOMINMAX
#include <DXGI.h>
#include <D3Dcommon.h>
#include <D3D11.h>

#include <D3D11SDKLayers.h>
#include <D3DCompiler.h>

#define DX11_EXPORT PLUGIN_EXPORT

#define SAFE_RELEASE(o) if (o != nullptr) { o->Release(); o = nullptr; }

DECLARE_LOG_CATEGORY_EXTERN(FLogDX11, Fatal);

class FDX11CommandBuffer;
class FDX11CommandBufferManager;
class FDX11Device;
class FDX11Driver;
class FDX11DriverList;
class FDX11EventQuery;
class FDX11RenderAPI;
class FDX11RenderWindow;
class FDX11RenderWindowManager;
class FDX11VideoMode;
class FDX11VideoOutputInfo;
class FDX11VideoModeInfo;