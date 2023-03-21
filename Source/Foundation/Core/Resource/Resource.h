#pragma once

#include "CorePrerequisites.h"
#include "FileSystem/Path.h"
#include "Misc/UUID.h"
#include "ResourceHandle.h"
#include "Resource.g.h"

QCLASS()
class DLL_EXPORT FResource : public QObject {
    GENERATED_BODY()

private:
    FString mName;

    FPath mPath;

    QPROPERTY()
    FUuid mUuid;

    bool bInitialized = false;
    bool mIsKeepSourceData;

public:
    virtual void initialize();
    virtual void destroy();

    void setName(const FString &name);
    const auto &getName() const { return mName; }

    void setPath(const FPath &path);
    const auto &getPath() const { return mPath; }

    void setUuid(const FUuid &uuid);
    const auto &getUuid() const { return mUuid; }


private:
    friend class FResources;
};

using HResource = FResourceHandle<FResource>;
