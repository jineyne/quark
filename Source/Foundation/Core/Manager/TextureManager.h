#pragma once

#include "CorePrerequisites.h"
#include "Image/Texture.h"
#include "Misc/Module.h"

class DLL_EXPORT FTextureManager : public TModule<FTextureManager> {
public:
    virtual ~FTextureManager() = default;

public:
    FTexture *createTexture(const FTextureDesc &desc);

protected:
    void onStartUp() override;
    void onShutDown() override;

    virtual FTexture *createTextureInternal(const FTextureDesc &desc) = 0;
};