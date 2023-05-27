#pragma once

#include "CorePrerequisites.h"
#include "CameraBase.h"

enum class ERenderLocation {
    Prepare,
    Overlay,
    Count,
};


class DLL_EXPORT RendererExtension {
private:
    ERenderLocation mLocation;
    uint32_t mPriority;

private:
    static void Initializer(RendererExtension *extension, const std::any &data);
    static void Deleter(RendererExtension *extension);

public:
    virtual void initialize(const std::any &data) {}
    virtual void destroy() {}

    virtual void render(CameraBase *camera) = 0;
    virtual bool checkCamera(CameraBase *camera) = 0;

    auto getPriority() const { return mPriority; }
    auto getLocation() const { return mLocation; }

protected:
    RendererExtension(ERenderLocation location, uint32_t priority)
    : mLocation(location), mPriority(priority) {}

    virtual ~RendererExtension() = default;

};
