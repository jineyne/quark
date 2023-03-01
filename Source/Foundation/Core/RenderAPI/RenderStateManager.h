#pragma once

#include "CorePrerequisites.h"
#include "Misc/Module.h"
#include "SamplerState.h"
#include "RenderStateManager.g.h"

QCLASS()
class DLL_EXPORT FRenderStateManager : public TModule<FRenderStateManager> {
    GENERATED_BODY()

private:
    mutable FSamplerState *mDefaultSamplerState = nullptr;
    mutable TMap<FSamplerStateDesc, FSamplerState*> mCachedSamplerStateMap;

public:
    FSamplerState *createSamplerState(const FSamplerStateDesc &desc) const;

    FSamplerState *getDefaultSamplerState() const;

protected:
    virtual void onShutDown() override;

    virtual FSamplerState *createSamplerStateInternal(const FSamplerStateDesc &desc) const;

private:
    void notifySamplerStateCreated(const FSamplerStateDesc &desc, FSamplerState *state) const;
    void notifySamplerStateDestroyed(const FSamplerStateDesc &desc) const;

    FSamplerState *findCachedState(const FSamplerStateDesc &desc) const;

private:
    friend class SamplerState;
};

DLL_EXPORT FRenderStateManager &gRenderStateManager();