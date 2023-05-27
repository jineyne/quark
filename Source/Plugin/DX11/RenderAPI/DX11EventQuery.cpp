#include "DX11EventQuery.h"

#include "DX11Device.h"
#include "DX11RenderAPI.h"
#include "DX11CommandBuffer.h"

DX11EventQuery::DX11EventQuery(uint32_t deviceIdx) {
    assert(deviceIdx == 0 && "Multiple GPUs not supported natively on DirectX 11.");

    DX11RenderAPI* rs = static_cast<DX11RenderAPI*>(RenderAPI::InstancePtr());
    DX11Device *device = rs->getPrimaryDevice();

    D3D11_QUERY_DESC queryDesc;
    queryDesc.Query = D3D11_QUERY_EVENT;
    queryDesc.MiscFlags = 0;

    HRESULT hr = device->getDevice()->CreateQuery(&queryDesc, &mQuery);
    if(hr != S_OK)
    {
        EXCEPT(LogDX11, RenderAPIException, TEXT("Failed to create an Event query."));
    }

    mContext = device->getImmediateContext();
}

DX11EventQuery::~DX11EventQuery() {
    SAFE_RELEASE(mQuery);
}

void DX11EventQuery::begin(CommandBuffer *cb) {
    auto execute = [&]() {
        mContext->End(mQuery);
        setActive(true);
    };

    if (cb == nullptr) {
        execute();
    } else {
        DX11CommandBuffer *d3d11cb = static_cast<DX11CommandBuffer *>(cb);
        d3d11cb->queueCommand(execute);
    }
}

bool DX11EventQuery::isReady() const {
    BOOL queryData;
    return mContext->GetData(mQuery, &queryData, sizeof(BOOL), 0) == S_OK;
}
