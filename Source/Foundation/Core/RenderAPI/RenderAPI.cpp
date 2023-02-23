#include "RenderAPI.h"

FRenderWindow *FRenderAPI::initialize(const FRenderWindowDesc &desc) {
    initialize();

    auto renderWindow = FRenderWindow::New(desc);
    initializeWithWindow(renderWindow);

    return renderWindow;
}

void FRenderAPI::initialize() {

}

void FRenderAPI::initializeWithWindow(FRenderWindow *window) {

}

FRenderAPI &gRenderAPI() {
    return FRenderAPI::Instance();
}
