#pragma once

#include "CorePrerequisites.h"
#include "Image/Texture.h"
#include "Misc/Module.h"

class DLL_EXPORT TextureManager : public TModule<TextureManager> {
public:
    virtual ~TextureManager() = default;

public:
    Texture *createTexture(const TextureDesc &desc);

protected:
    void onStartUp() override;
    void onShutDown() override;

    virtual Texture *createTextureInternal(const TextureDesc &desc) = 0;
};