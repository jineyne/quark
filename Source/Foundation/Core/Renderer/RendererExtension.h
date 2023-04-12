#pragma once

#include "CorePrerequisites.h"
#include "CameraBase.h"

enum class ERenderLocation {
    Prepare,
    Overlay,
    Count,
};


class DLL_EXPORT FRendererExtension {
private:
    ERenderLocation mLocation;
    uint32_t mPriority;

private:
    static void Initializer(FRendererExtension *extension, const std::any &data);
    static void Deleter(FRendererExtension *extension);

public:
    virtual void initialize(const std::any &data) {}
    virtual void destroy() {}

    virtual void render(FCameraBase *camera) = 0;
    virtual bool checkCamera(FCameraBase *camera) = 0;

    auto getPriority() const { return mPriority; }
    auto getLocation() const { return mLocation; }

protected:
    FRendererExtension(ERenderLocation location, uint32_t priority)
    : mLocation(location), mPriority(priority) {}

    virtual ~FRendererExtension() = default;

};
