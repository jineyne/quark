#include "Archive.h"

#include "Reflection/Class.h"
#include "Reflection/Property.h"

FArchive::FArchive(const TSharedPtr<FStream> &target, EArchiveMode mode) : mTarget(target), mMode(mode) {}

FArchive::operator bool() {
    return !mTarget->eof();
}

void FArchive::reset() {
    mTarget->seek(0);
}