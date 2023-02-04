#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Archive.h"

class DLL_EXPORT FTextArchive : public FArchive {
public:
    FTextArchive(const TSharedPtr<FStream> &target, EArchiveMode mode);

public:
    FArchive &operator<<(bool &value) override;
    FArchive &operator<<(int8_t &value) override;
    FArchive &operator<<(uint8_t &value) override;
    FArchive &operator<<(int32_t &value) override;
    FArchive &operator<<(uint32_t &value) override;
    FArchive &operator<<(int64_t &value) override;
    FArchive &operator<<(uint64_t &value) override;

    FArchive &operator<<(float &value) override;
    FArchive &operator<<(double &value) override;

    FArchive &operator<<(FString &value) override;
};