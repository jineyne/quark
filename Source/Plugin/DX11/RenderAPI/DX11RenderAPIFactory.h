#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/RenderAPIFactory.h"
#include "Manager/RenderAPIManager.h"

class DX11_EXPORT FDX11RenderAPIFactory : public FRenderAPIFactory {
private:
    class InitOnStart {
    public:
        InitOnStart() {
            static FRenderAPIFactory *factory;
            if (factory == nullptr) {
                factory = new FDX11RenderAPIFactory();
                FRenderAPIManager::RegisterFactory(factory);
            }
        }
    };

    static InitOnStart initOnStart;

public:
    static constexpr const TCHAR *Name = TEXT("quark-dx11");

public:
    void create() override;
    const TCHAR *name() const override { return Name; }
};