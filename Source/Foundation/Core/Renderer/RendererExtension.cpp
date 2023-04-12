#include "RendererExtension.h"
#include "Renderer.h"

void FRendererExtension::Initializer(FRendererExtension *extension, const std::any &data) {
    gRenderer().addPlugin(extension);
    extension->initialize(data);
}

void FRendererExtension::Deleter(FRendererExtension *extension) {
    gRenderer().removePlugin(extension);

    extension->destroy();
    extension->~FRendererExtension();

    q_free(extension);
}

