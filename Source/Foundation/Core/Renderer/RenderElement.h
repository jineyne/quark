#pragma once

#include "CorePrerequisites.h"
#include "Material/GpuParamsSet.h"
#include "Material/Material.h"
#include "Mesh/Mesh.h"

class DLL_EXPORT RenderElement {
public:
    Mesh *mesh;
    Material *material;
    uint32_t defaultTechniqueIdx = 0;
    uint32_t writeVelocityTechniqueIdx = static_cast<uint32_t>(-1);
    GpuParamsSet *params;
    uint32_t type;
    virtual void draw() const = 0;

protected:
    ~RenderElement() = default;
};