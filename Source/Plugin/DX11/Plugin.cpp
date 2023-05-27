#include "DX11Prerequisites.h"
#include "RenderAPI/DX11RenderAPIFactory.h"

DEFINE_LOG_CATEGORY(LogDX11)

extern "C" DX11_EXPORT const TCHAR *getPluginName() {
    return DX11RenderAPIFactory::Name;
}
