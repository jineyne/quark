#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/RenderAPIFactory.h"
#include "Manager/RenderAPIManager.h"

class DX11_EXPORT DX11RenderAPIFactory : public RenderAPIFactory {
private:
    class InitOnStart {
    public:
        InitOnStart() {
            static RenderAPIFactory *factory;
            if (factory == nullptr) {
                factory = q_new<DX11RenderAPIFactory>();
                RenderAPIManager::RegisterFactory(factory);
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