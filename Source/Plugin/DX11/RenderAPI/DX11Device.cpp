#include "DX11Device.h"
#include "Exception/Exception.h"

FDX11Device::FDX11Device(ID3D11Device *device) : mDevice(device) {
    HRESULT hr;

    if (device) {
        device->GetImmediateContext(&mImmediateContext);

        hr = device->CreateClassLinkage(&mClassLinkage);
        if (FAILED(hr)) {
            EXCEPT(FLogDX11, RenderAPIException, TEXT("Unable to create class linkage"));
        }

#if DEBUG_MODE
        hr = device->QueryInterface(__uuidof(ID3D11Debug), (LPVOID *) &mDebug);
        if (FAILED(hr)) {
            EXCEPT(FLogDX11, RenderAPIException, TEXT("Unable to query ID3D11Debug"));
        }

        hr = mDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        if (FAILED(hr)) {
            EXCEPT(FLogDX11, RenderAPIException, TEXT("Unable to set ReportLiveDeviceObjects"));
        }

        hr = device->QueryInterface(__uuidof(ID3D11InfoQueue), (LPVOID *) &mInfoQueue);
        if (FAILED(hr)) {
            EXCEPT(FLogDX11, RenderAPIException, TEXT("Unable to query ID3D11InfoQueue"));
        }

        setExceptionsErrorLevel(ELogLevel::Error);
#endif
    }
}

FDX11Device::~FDX11Device() {
    mDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

    if (mImmediateContext) {
        mImmediateContext->Flush();
        mImmediateContext->ClearState();
    }

    SAFE_RELEASE(mInfoQueue);
    SAFE_RELEASE(mDevice);
    SAFE_RELEASE(mImmediateContext);
}

FDX11Device *FDX11Device::New(ID3D11Device *device) {
    return new FDX11Device(device);
}

FString FDX11Device::getErrorDescription(bool doClearErrors) {
    if (mDevice == nullptr)
        return "Null device.";

    FString res;

    if (mInfoQueue != nullptr) {
        UINT64 numStoredMessages = mInfoQueue->GetNumStoredMessagesAllowedByRetrievalFilter();
        for (UINT64 i = 0; i < numStoredMessages; i++) {
            // Get the size of the message
            SIZE_T messageLength = 0;
            mInfoQueue->GetMessage(i, nullptr, &messageLength);
            // Allocate space and get the message
            D3D11_MESSAGE *pMessage = (D3D11_MESSAGE *) malloc(messageLength);
            mInfoQueue->GetMessage(i, pMessage, &messageLength);
            res = res + pMessage->pDescription + "\n";
            free(pMessage);
        }
    }

    if (doClearErrors) {
        clearErrors();
    }

    return res;
}

bool FDX11Device::hasError() const {
    if (mInfoQueue != nullptr) {
        const uint64_t numStoredMessages = mInfoQueue->GetNumStoredMessagesAllowedByRetrievalFilter();
        return numStoredMessages > 0;
    }

    return false;
}

void FDX11Device::clearErrors() {
    if (mDevice != nullptr && mInfoQueue != nullptr) {
        mInfoQueue->ClearStoredMessages();
    }
}

void FDX11Device::setExceptionsErrorLevel(ELogLevel exceptionsErrorLevel) {
    if(mInfoQueue == nullptr)
        return;

    mInfoQueue->ClearRetrievalFilter();
    mInfoQueue->ClearStorageFilter();

    D3D11_INFO_QUEUE_FILTER filter;
    ZeroMemory(&filter, sizeof(D3D11_INFO_QUEUE_FILTER));
    TArray<D3D11_MESSAGE_SEVERITY> severityList;

    switch(exceptionsErrorLevel) {
        case ELogLevel::Info:
            severityList.add(D3D11_MESSAGE_SEVERITY_INFO);
        case ELogLevel::Warning:
            severityList.add(D3D11_MESSAGE_SEVERITY_WARNING);
        case ELogLevel::Error:
            severityList.add(D3D11_MESSAGE_SEVERITY_ERROR);
        case ELogLevel::Fatal:
            severityList.add(D3D11_MESSAGE_SEVERITY_CORRUPTION);
        default:
            break;
    }

    if (severityList.length() > 0) {
        filter.AllowList.NumSeverities = (UINT)severityList.length();
        filter.AllowList.pSeverityList = &severityList[0];
    }

    mInfoQueue->AddStorageFilterEntries(&filter);
    mInfoQueue->AddRetrievalFilterEntries(&filter);
}


