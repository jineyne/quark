#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Archive.h"

class DLL_EXPORT TextArchive : public Archive {
public:
    TextArchive(const TSharedPtr<Stream> &target, EArchiveMode mode);

public:
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

    Archive &operator<<(struct Struct *value) override;
    Archive &operator<<(struct Object *value) override;
};