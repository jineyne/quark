#include "InputType.h"

#include "Manager/InputManager.h"

IInputEventListener::~IInputEventListener() {
    gInputManager().removeEventListener(this);
}

ITouchEventListener::~ITouchEventListener() {
    gInputManager().removeEventListener(this);
}
