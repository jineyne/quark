#include "DX11Prerequisites.h"
#include "RenderAPI/DX11RenderAPIFactory.h"

DEFINE_LOG_CATEGORY(FLogDX11)

extern "C" DX11_EXPORT const TCHAR *getPluginName() {
    return FDX11RenderAPIFactory::Name;
}
