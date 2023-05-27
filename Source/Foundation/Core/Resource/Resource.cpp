#include "Resource.h"

void Resource::initialize() {

}

void Resource::destroy() {

}

void Resource::setName(const String &name) {
    mName = name;
}

void Resource::setPath(const Path &path) {
    mPath = path;
}

void Resource::setUuid(const Uuid &uuid) {
    mUuid = uuid;
}
