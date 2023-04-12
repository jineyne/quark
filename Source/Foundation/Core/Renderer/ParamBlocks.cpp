#include "ParamBlocks.h"

TArray<FParamBlock *> FParamBlockManager::ToInitialize;

FParamBlockManager::FParamBlockManager() {
    for (auto &entry : ToInitialize) {
        entry->initialize();
    }

    ToInitialize.clear();
}

void FParamBlockManager::RegisterBlock(FParamBlock *paramBlock) {
    if (IsRunning()) {
        paramBlock->initialize();
    } else {
        ToInitialize.add(paramBlock);
    }
}

void FParamBlockManager::UnregisterBlock(FParamBlock *paramBlock) {
    auto it = ToInitialize.find(paramBlock);
    if (it != INDEX_NONE) {
        ToInitialize.remove(paramBlock);
    }
}
