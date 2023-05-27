#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/RenderWindow.h"
#include "Misc/Module.h"
#include "RenderWindowManager.g.h"

QCLASS(abstract)
class DLL_EXPORT RenderWindowManager : public TModule<RenderWindowManager> {
    GENERATED_BODY();

protected:
    using WindowMap = TMap<uint32_t, RenderWindow *>;
    uint32_t mNextWindowID = 0;
    WindowMap mWindowMap;

    TArray<RenderWindow *> mCloseRequestedWindowList;

public:
    virtual ~RenderWindowManager() = default;

public:
    RenderWindow *create(const RenderWindowDesc &desc, RenderWindow *parent = nullptr);

    virtual void update();

    RenderWindow *getWindow(uint32_t id);

    void notifyWindowDestroyed(RenderWindow *window);
    void notifyCloseRequested(RenderWindow *window);

protected:
    virtual RenderWindow *createInternal(const RenderWindowDesc &desc, uint32_t windowId, RenderWindow *parent) = 0;
};

DLL_EXPORT RenderWindowManager &gRenderWindowManager();
