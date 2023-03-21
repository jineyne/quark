#include "Resource.h"

void FResource::initialize() {

}

void FResource::destroy() {

}

void FResource::setName(const FString &name) {
    mName = name;
}

void FResource::setPath(const FPath &path) {
    mPath = path;
}

void FResource::setUuid(const FUuid &uuid) {
    mUuid = uuid;
}
