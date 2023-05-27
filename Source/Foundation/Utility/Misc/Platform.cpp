#include "Platform.h"

TArray<IPlatformEventHandlerBase *> Platform::PlatformEventHandlerList;

void Platform::RegisterPlatformEventHandler(IPlatformEventHandlerBase *handler) {
    PlatformEventHandlerList.add(handler);
}

void Platform::UnRegisterPlatformEventHandler(IPlatformEventHandlerBase *handler) {
    PlatformEventHandlerList.remove(handler);
}