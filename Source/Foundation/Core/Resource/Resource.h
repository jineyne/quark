#pragma once

#include "CorePrerequisites.h"
#include "FileSystem/Path.h"
#include "Misc/Uuid.h"
#include "ResourceHandle.h"
#include "Resource.g.h"

QCLASS()
class DLL_EXPORT Resource : public Object {
    GENERATED_BODY()

private:
    String mName;

    Path mPath;

    QPROPERTY()
    Uuid mUuid;

    bool bInitialized = false;
    bool mIsKeepSourceData;

public:
    virtual void initialize();
    virtual void destroy();

    void setName(const String &name);
    const auto &getName() const { return mName; }

    void setPath(const Path &path);
    const auto &getPath() const { return mPath; }

    void setUuid(const Uuid &uuid);
    const auto &getUuid() const { return mUuid; }


private:
    friend class Resources;
};

using HResource = FResourceHandle<Resource>;
