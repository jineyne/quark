#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/RenderWindow.h"
#include "Misc/Module.h"
#include "RenderWindowManager.g.h"

QCLASS(abstract)
class DLL_EXPORT FRenderWindowManager : public TModule<FRenderWindowManager> {
    GENERATED_BODY();

protected:
    using WindowMap = TMap<uint32_t, FRenderWindow *>;
    uint32_t mNextWindowID = 0;
    WindowMap mWindowMap;

    TArray<FRenderWindow *> mCloseRequestedWindowList;

public:
    virtual ~FRenderWindowManager() = default;

public:
    FRenderWindow *create(const FRenderWindowDesc &desc, FRenderWindow *parent = nullptr);

    virtual void update();

    FRenderWindow *getWindow(uint32_t id);

    void notifyWindowDestroyed(FRenderWindow *window);
    void notifyCloseRequested(FRenderWindow *window);

protected:
    virtual FRenderWindow *createInternal(const FRenderWindowDesc &desc, uint32_t windowId, FRenderWindow *parent) = 0;
};

DLL_EXPORT FRenderWindowManager &gRenderWindowManager();
