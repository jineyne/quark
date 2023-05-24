#pragma once

#include "Prerequisites.h"
#include "CoreApplication.h"
#include "Application.g.h"

QCLASS()
class DLL_EXPORT QApplication : public QCoreApplication {
    GENERATED_BODY()

protected:
    void onStartUp() override;
    void onShutDown() override;
};
