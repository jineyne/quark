#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

class Stream;
class String;

enum class EArchiveMode {
    Load,
    Save,
};

class DLL_EXPORT Archive {
private:
    TSharedPtr<Stream> mTarget;
    EArchiveMode mMode;

public:
    Archive(const TSharedPtr<Stream> &target, EArchiveMode mode);
    virtual ~Archive() = default;

public:
    virtual operator bool();

    // methods for archive data
    virtual void serialize(void *data, size_t size) = 0;

    virtual Archive &operator<<(bool &value) = 0;
    virtual Archive &operator<<(int8_t &value) = 0;
    virtual Archive &operator<<(uint8_t &value) = 0;
    virtual Archive &operator<<(int32_t &value) = 0;
    virtual Archive &operator<<(uint32_t &value) = 0;
    virtual Archive &operator<<(int64_t &value) = 0;
    virtual Archive &operator<<(uint64_t &value) = 0;

    virtual Archive &operator<<(float &value) = 0;
    virtual Archive &operator<<(double &value) = 0;

    virtual Archive &operator<<(String &value) = 0;

    // methods for meta data

    bool isSaving() const { return mMode == EArchiveMode::Save; }
    bool isLoading() const { return mMode == EArchiveMode::Load; }

    TSharedPtr<Stream> getTarget() const { return mTarget; }

public:
    void reset();
};
