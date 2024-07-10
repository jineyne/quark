#include "Application.h"

Application::Application(const ApplicationStartUpDesc &desc) : CoreApplication(desc) {}


void Application::onStartUp() {
    CoreApplication::onStartUp();
}

void Application::onShutDown() {
    CoreApplication::onShutDown();
}
