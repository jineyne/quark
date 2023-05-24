#pragma once

#include "CorePrerequisites.h"
#include "Event/Event.h"
#include "Misc/Module.h"
#include "Input/InputDevice.h"
#include "Input/InputType.h"
#include "InputManager.g.h"

QCLASS()
class DLL_EXPORT FInputManager : public TModule<FInputManager> {
    GENERATED_BODY()

private:
    TArray<FInputDevice *> mInputDeviceList;

    TArray<IInputEventListener *> mInputEventListenerList;
    TArray<ITouchEventListener *> mTouchEventListenerList;

public:
    bool inputState(const FString &name, EInputState state);

    void addEventListener(IInputEventListener *listener);
    void addEventListener(ITouchEventListener *listener);
    void removeEventListener(IInputEventListener *listener);
    void removeEventListener(ITouchEventListener *listener);

    void postInputEvent(const FInputEvent &event, bool force = false);
    void postUnicodeEvent(const FUnicodeEvent &event, bool force = false);
    void postTouchEvent(const FTouchEvent &event, bool force = false);
    virtual void update();

    virtual void clearKeyState();

    virtual bool addInputDevice(FInputDevice *inputDevice);
    virtual bool removeInputDevice(FInputDevice *inputDevice);

private:
    void onShutDown() override;

    bool sendEventToListeners(const FInputEvent& event);
    bool sendEventToListeners(const FUnicodeEvent& event);
};

DLL_EXPORT FInputManager &gInputManager();