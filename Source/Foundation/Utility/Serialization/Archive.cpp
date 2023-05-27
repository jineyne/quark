#include "Archive.h"

#include "Reflection/Class.h"
#include "Reflection/Property.h"

Archive::Archive(const TSharedPtr<Stream> &target, EArchiveMode mode) : mTarget(target), mMode(mode) {}

Archive::operator bool() {
    return !mTarget->eof();
}

void Archive::reset() {
    mTarget->seek(0);
}