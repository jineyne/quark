#pragma once

#include "CorePrerequisites.h"
#include "Event/Event.h"
#include "Misc/Module.h"
#include "Input/InputDevice.h"
#include "Input/InputType.h"
#include "InputManager.g.h"

QCLASS()
class DLL_EXPORT InputManager : public TModule<InputManager> {
    GENERATED_BODY()

private:
    TArray<InputDevice *> mInputDeviceList;

    TArray<IInputEventListener *> mInputEventListenerList;
    TArray<ITouchEventListener *> mTouchEventListenerList;

public:
    bool inputState(const String &name, EInputState state);

    void addEventListener(IInputEventListener *listener);
    void addEventListener(ITouchEventListener *listener);
    void removeEventListener(IInputEventListener *listener);
    void removeEventListener(ITouchEventListener *listener);

    void postInputEvent(const InputEvent &event, bool force = false);
    void postUnicodeEvent(const UnicodeEvent &event, bool force = false);
    void postTouchEvent(const TouchEvent &event, bool force = false);
    virtual void update();

    virtual void clearKeyState();

    virtual bool addInputDevice(InputDevice *inputDevice);
    virtual bool removeInputDevice(InputDevice *inputDevice);

private:
    void onShutDown() override;

    bool sendEventToListeners(const InputEvent& event);
    bool sendEventToListeners(const UnicodeEvent& event);
};

DLL_EXPORT InputManager &gInputManager();