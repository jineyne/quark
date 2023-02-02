#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

class FStream;
class FString;

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

    // methods for class. ex) yaml, xml

    virtual void enter() { };
    virtual void exit() { };

    // methods for archive data

    virtual FArchive &operator<<(bool &value) = 0;
    virtual FArchive &operator<<(int8_t &value) = 0;
    virtual FArchive &operator<<(uint8_t &value) = 0;
    virtual FArchive &operator<<(int32_t &value) = 0;
    virtual FArchive &operator<<(uint32_t &value) = 0;
    virtual FArchive &operator<<(int64_t &value) = 0;
    virtual FArchive &operator<<(uint64_t &value) = 0;

    virtual FArchive &operator<<(float &value) = 0;
    virtual FArchive &operator<<(double &value) = 0;

    virtual FArchive &operator<<(FString &value) = 0;

    // require ?

    virtual FArchive &operator<<(class QObject *value) = 0;

    // methods for meta data

    bool isSaving() const { return mMode == EArchiveMode::Save; }
    bool isLoading() const { return mMode == EArchiveMode::Load; }

    TSharedPtr<FStream> getTarget() const { return mTarget; }

public:
    void reset();
};
