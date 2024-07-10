#pragma once

#include "Prerequisites.h"
#include "CoreApplication.h"
#include "Application.g.h"

QCLASS()
class DLL_EXPORT Application : public CoreApplication {
    GENERATED_BODY()

public:
    Application() = default;

    Application(const ApplicationStartUpDesc &desc);

protected:
    void onStartUp() override;
    void onShutDown() override;
};
