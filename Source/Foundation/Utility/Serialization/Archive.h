#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "FileSystem/Stream.h"

class DLL_EXPORT FArchive {
private:
    TSharedPtr<FStream> mTarget;
    bool bIsSaving;

public:
    FArchive(const TSharedPtr<FStream> &target, bool saving);

public:
    virtual FArchive &operator<<(bool &value);
    virtual FArchive &operator<<(int8_t &value);
    virtual FArchive &operator<<(uint8_t &value);
    virtual FArchive &operator<<(int32_t &value);
    virtual FArchive &operator<<(uint32_t &value);
    virtual FArchive &operator<<(int64_t &value);
    virtual FArchive &operator<<(uint64_t &value);
    virtual FArchive &operator<<(FString &value);

    virtual FArchive &operator<<(class QObject *value);

    bool isSaving() const { return bIsSaving; }
    bool isLoading() const { return !bIsSaving; }

public:
    void reset();
};
