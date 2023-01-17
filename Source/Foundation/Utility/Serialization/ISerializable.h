#pragma once

#include "Archive.h"

class ISerializable {
public:
    virtual void serialize(FArchive &ar) = 0;
};