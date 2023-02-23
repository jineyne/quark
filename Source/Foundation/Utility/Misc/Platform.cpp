#include "Platform.h"

TArray<IPlatformEventHandlerBase *> FPlatform::PlatformEventHandlerList;

void FPlatform::RegisterPlatformEventHandler(IPlatformEventHandlerBase *handler) {
    PlatformEventHandlerList.add(handler);
}

void FPlatform::UnRegisterPlatformEventHandler(IPlatformEventHandlerBase *handler) {
    PlatformEventHandlerList.remove(handler);
}