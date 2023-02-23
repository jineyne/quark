#pragma once

#include "DX11Prerequisites.h"

class DX11_EXPORT FDX11Device {
private:
    ID3D11Device *mDevice;
    ID3D11DeviceContext *mImmediateContext;
    ID3D11ClassLinkage  *mClassLinkage;

    ID3D11Debug *mDebug;
    ID3D11InfoQueue *mInfoQueue;

public:
    FDX11Device(ID3D11Device *device);
    ~FDX11Device();

public:
    static FDX11Device *New(ID3D11Device *device);

public:
    FString getErrorDescription(bool doClearErrors = true);
    bool hasError() const;

    ID3D11Device *getDevice() const { return mDevice; }
    ID3D11DeviceContext *getImmediateContext() const { return mImmediateContext; }
    ID3D11ClassLinkage  *getClassLinkage() const { return mClassLinkage; }

private:
    void clearErrors();
    void setExceptionsErrorLevel(ELogLevel exceptionsErrorLevel);
};
