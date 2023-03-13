#include "DX11InputLayoutManager.h"
#include "Misc/Misc.h"
#include "Utility/DX11Mapper.h"
#include "RenderAPI/DX11Device.h"
#include "RenderAPI/DX11RenderAPI.h"
#include "Exception/Exception.h"

bool FDX11InputLayoutManager::EqualFunc::operator()(const FDX11InputLayoutManager::VertexDeclarationKey &a,
                                                    const FDX11InputLayoutManager::VertexDeclarationKey &b) const {
    if (a.vertxDeclId != b.vertxDeclId)
        return false;

    if (a.vertexProgramId != b.vertexProgramId)
        return false;

    return true;
}

size_t FDX11InputLayoutManager::HashFunc::operator()(const FDX11InputLayoutManager::VertexDeclarationKey &key) const {
    size_t hash = 0;

    CombineHash(hash, key.vertxDeclId);
    CombineHash(hash, key.vertexProgramId);

    return hash;
}

FDX11InputLayoutManager::~FDX11InputLayoutManager() {
    while (mInputLayoutMap.begin() != mInputLayoutMap.end()) {
        auto firstElem = mInputLayoutMap.begin();

        SAFE_RELEASE(firstElem->second->inputLayout);
        delete firstElem->second;

        mInputLayoutMap.remove(firstElem->first);
    }
}

ID3D11InputLayout *
FDX11InputLayoutManager::retrieveInputLayout(FVertexDeclaration *vertexShaderDecl, FVertexDeclaration *vertexBufferDecl,
                                             FDX11GpuProgram *vertexProgram) {
    VertexDeclarationKey pair;
    pair.vertxDeclId = vertexBufferDecl->getId();
    pair.vertexProgramId = vertexProgram->getProgramId();

    auto item = mInputLayoutMap.find(pair);
    if (item == nullptr) {
        if (mInputLayoutMap.length() >= DECLARATION_BUFFER_SIZE)
            removeLeastUsed(); // Prune so the buffer doesn't just infinitely grow

        addNewInputLayout(vertexShaderDecl, vertexBufferDecl, vertexProgram);

        item = mInputLayoutMap.find(pair);

        if (item == nullptr) // We failed to create input layout
            return nullptr;
    }

    (*item)->lastUsedIdx = ++mLastUsedCounter;
    return (*item)->inputLayout;
}

void
FDX11InputLayoutManager::addNewInputLayout(FVertexDeclaration *vertexShaderDecl, FVertexDeclaration *vertexBufferDecl,
                                           FDX11GpuProgram *vertexProgram) {
    TArray<D3D11_INPUT_ELEMENT_DESC> declElements;

    const TArray<FVertexElement> &bufferElems = vertexBufferDecl->getElements();
    const TArray<FVertexElement> &shaderElems = vertexShaderDecl->getElements();

    int32_t maxStreamIdx = -1;
    for (auto iter = bufferElems.begin(); iter != bufferElems.end(); ++iter) {
        declElements.add(D3D11_INPUT_ELEMENT_DESC());
        D3D11_INPUT_ELEMENT_DESC &elementDesc = declElements.top();

        elementDesc.SemanticName = FDX11Mapper::Get(iter->getSemantic());
        elementDesc.SemanticIndex = iter->getSemanticIndex();
        elementDesc.Format = FDX11Mapper::Get(iter->getType());
        elementDesc.InputSlot = iter->getSemanticIndex();
        elementDesc.AlignedByteOffset = static_cast<WORD>(iter->getOffset()) == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;

        if (iter->getInstanceStepRate() == 0) {
            elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            elementDesc.InstanceDataStepRate = 0;
        } else {
            elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
            elementDesc.InstanceDataStepRate = iter->getInstanceStepRate();
        }

        maxStreamIdx = std::max(maxStreamIdx, static_cast<int32_t>(iter->getSemanticIndex()));
    }

    // Find elements missing in buffer and add a dummy stream for them
    for (auto shaderIter = shaderElems.begin(); shaderIter != shaderElems.end(); ++shaderIter) {
        bool foundElement = false;
        for (auto bufferIter = bufferElems.begin(); bufferIter != bufferElems.end(); ++bufferIter) {
            if (shaderIter->getSemantic() == bufferIter->getSemantic() &&
                shaderIter->getSemanticIndex() == bufferIter->getSemanticIndex()) {
                foundElement = true;
                break;
            }
        }

        if (!foundElement) {
            declElements.add(D3D11_INPUT_ELEMENT_DESC());
            D3D11_INPUT_ELEMENT_DESC &elementDesc = declElements.top();

            elementDesc.SemanticName = FDX11Mapper::Get(shaderIter->getSemantic());
            elementDesc.SemanticIndex = shaderIter->getSemanticIndex();
            elementDesc.Format = FDX11Mapper::Get(shaderIter->getType());
            elementDesc.InputSlot = (UINT32) (maxStreamIdx + 1);
            elementDesc.AlignedByteOffset = 0;
            elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            elementDesc.InstanceDataStepRate = 0;
        }
    }

    FDX11RenderAPI *d3d11rs = static_cast<FDX11RenderAPI *>(FRenderAPI::InstancePtr());
    FDX11Device *device = d3d11rs->getPrimaryDevice();

    const FDataBlob &microcode = vertexProgram->getMicroCode();

    InputLayoutEntry *newEntry = new InputLayoutEntry();
    newEntry->lastUsedIdx = ++mLastUsedCounter;
    newEntry->inputLayout = nullptr;
    HRESULT hr = device->getDevice()->CreateInputLayout(
            &declElements[0],
            (UINT32) declElements.length(),
            microcode.data,
            microcode.size,
            &newEntry->inputLayout);

    if (FAILED(hr) || device->hasError()) {
        EXCEPT(FLogDX11, RenderAPIException, TEXT("Unable to set D3D11 vertex declaration %ls"), *device->getErrorDescription());
    }


    // Create key and add to the layout map
    VertexDeclarationKey pair;
    pair.vertxDeclId = vertexBufferDecl->getId();
    pair.vertexProgramId = vertexProgram->getProgramId();

    mInputLayoutMap[pair] = newEntry;
}

void FDX11InputLayoutManager::removeLeastUsed() {
    if (!mWarningShown) {
        LOG(FLogDX11, Warning, TEXT("Input layout buffer is full, pruning last {0} elements. This is probably okay "
                               "unless you are creating a massive amount of input layouts as they will get re-created every frame. "
                               "In that case you should increase the layout buffer size. This warning won't be shown again."),
                               NUM_ELEMENTS_TO_PRUNE);

        mWarningShown = true;
    }

    TMap<UINT32, VertexDeclarationKey> leastFrequentlyUsedMap;

    for (auto iter = mInputLayoutMap.begin(); iter != mInputLayoutMap.end(); ++iter)
        leastFrequentlyUsedMap[iter->second->lastUsedIdx] = iter->first;

    UINT32 elemsRemoved = 0;
    for (auto iter = leastFrequentlyUsedMap.begin(); iter != leastFrequentlyUsedMap.end(); ++iter) {
        auto inputLayoutIter = mInputLayoutMap.find(iter->second);

        SAFE_RELEASE((*inputLayoutIter)->inputLayout);
        delete *inputLayoutIter;

        mInputLayoutMap.remove(iter->second);

        elemsRemoved++;
        if (elemsRemoved >= NUM_ELEMENTS_TO_PRUNE) {
            break;
        }
    }
}
