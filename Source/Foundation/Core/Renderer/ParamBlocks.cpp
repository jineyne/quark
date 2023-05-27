#include "ParamBlocks.h"

TArray<ParamBlock *> ParamBlockManager::ToInitialize;

ParamBlockManager::ParamBlockManager() {
    for (auto &entry : ToInitialize) {
        entry->initialize();
    }

    ToInitialize.clear();
}

void ParamBlockManager::RegisterBlock(ParamBlock *paramBlock) {
    if (IsRunning()) {
        paramBlock->initialize();
    } else {
        ToInitialize.add(paramBlock);
    }
}

void ParamBlockManager::UnregisterBlock(ParamBlock *paramBlock) {
    auto it = ToInitialize.find(paramBlock);
    if (it != INDEX_NONE) {
        ToInitialize.remove(paramBlock);
    }
}
