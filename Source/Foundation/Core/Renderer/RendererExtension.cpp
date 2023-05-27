#include "RendererExtension.h"
#include "Renderer.h"

void RendererExtension::Initializer(RendererExtension *extension, const std::any &data) {
    gRenderer().addPlugin(extension);
    extension->initialize(data);
}

void RendererExtension::Deleter(RendererExtension *extension) {
    gRenderer().removePlugin(extension);

    extension->destroy();
    extension->~RendererExtension();

    q_free(extension);
}

