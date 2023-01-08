#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "FileSystem/Stream.h"

enum class EArchiveMode {
    Load,
    Save,
};

class DLL_EXPORT FArchive {
private:
    TSharedPtr<FStream> mTarget;
    EArchiveMode mMode;

public:
    FArchive(const TSharedPtr<FStream> &target, EArchiveMode mode);

public:
    virtual operator bool();

    virtual FArchive &operator<<(bool &value);
    virtual FArchive &operator<<(int8_t &value);
    virtual FArchive &operator<<(uint8_t &value);
    virtual FArchive &operator<<(int32_t &value);
    virtual FArchive &operator<<(uint32_t &value);
    virtual FArchive &operator<<(int64_t &value);
    virtual FArchive &operator<<(uint64_t &value);

    virtual FArchive &operator<<(float &value);
    virtual FArchive &operator<<(double &value);

    virtual FArchive &operator<<(FString &value);

    virtual FArchive &operator<<(class QObject *value);

    bool isSaving() const { return mMode == EArchiveMode::Save; }
    bool isLoading() const { return mMode == EArchiveMode::Load; }

public:
    void reset();
};
