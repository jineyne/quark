#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Archive.h"

class DLL_EXPORT BinaryArchive : public Archive {
public:
    BinaryArchive(const TSharedPtr<Stream> &target, EArchiveMode mode);

public:
    void serialize(void *data, size_t size) override;

    Archive &operator<<(bool &value) override;
    Archive &operator<<(int8_t &value) override;
    Archive &operator<<(uint8_t &value) override;
    Archive &operator<<(int32_t &value) override;
    Archive &operator<<(uint32_t &value) override;
    Archive &operator<<(int64_t &value) override;
    Archive &operator<<(uint64_t &value) override;

    Archive &operator<<(float &value) override;
    Archive &operator<<(double &value) override;

    Archive &operator<<(String &value) override;
};