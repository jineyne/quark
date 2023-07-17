#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/String.h"
#include "Exception/Exception.h"
#include "Logging/LogDefines.h"

DLL_EXPORT DECLARE_LOG_CATEGORY_EXTERN(LogModule, Info)

template <typename T>
class TModule : public Object {
public:
    static T &Instance() {
        if (!IsStartedUp()) {
            EXCEPT(LogModule, InternalErrorException, TEXT("Trying to access a module but it hasn't been started up yet."));
        }

        if (IsDestroyed()) {
            EXCEPT(LogModule, InternalErrorException, TEXT("Trying to access a destroyed module."));
        }

        return *InstanceInternal();
    }

    static T *InstancePtr() {
        if (!IsStartedUp()) {
            EXCEPT(LogModule, InternalErrorException, TEXT("Trying to access a module but it hasn't been started up yet."));
        }

        if (IsDestroyed()) {
            EXCEPT(LogModule, InternalErrorException, TEXT("Trying to access a destroyed module."));
        }

        return InstanceInternal();
    }

    template <class ...Args>
    static void StartUp(Args &&...args) {
        if (IsStartedUp()) {
            EXCEPT(LogModule, InternalErrorException, TEXT("Trying to start an already started module."));
        }

        if (IsDestroyed()) {
            EXCEPT(LogModule, InternalErrorException, TEXT("Trying to shut down an already shut down module."));
        }

        InstanceInternal() = q_new<T>(std::forward<Args>(args)...);
        static_cast<TModule *>(InstanceInternal())->onStartUp();

        LOG(LogModule, Debug, TEXT("Module '%ls' start up"), *T::StaticClass()->getName());
        IsStartedUp() = true;
        IsDestroyed() = false;
    }

    template <typename U, class ...Args>
    static void StartUp(Args &&...args) {
        static_assert(std::is_base_of<T, U>::value, "Provided type is not derived from type the Module is initialized with.");

        if (IsStartedUp()) {
            EXCEPT(LogModule, InternalErrorException, TEXT("Trying to start an already started module."));
        }

        if (IsDestroyed()) {
            EXCEPT(LogModule, InternalErrorException, TEXT("Trying to shut down an already shut down module."));
        }

        InstanceInternal() = q_new<U>(std::forward<Args>(args)...);
        static_cast<TModule *>(InstanceInternal())->onStartUp();

        LOG(LogModule, Debug, TEXT("Module '%ls' start up with '%ls'"), *U::StaticClass()->getName(), *U::StaticClass()->getName());
        IsStartedUp() = true;
        IsDestroyed() = false;
    }

    template <class ...Args>
    static void ShutDown(Args &&...args) {
        if (!IsStartedUp()) {
            EXCEPT(LogModule, InternalErrorException, TEXT("Trying to shut down a module which was never started."));
        }

        if (IsDestroyed()) {
            EXCEPT(LogModule, InternalErrorException, TEXT("Trying to shut down an already shut down module."));
        }

        static_cast<TModule *>(InstanceInternal())->onShutDown();
        q_delete(InstanceInternal());

        LOG(LogModule, Debug, TEXT("Module '%s' shutdown"), *T::StaticClass()->getName());
        InstanceInternal() = nullptr;
        // IsDestroyed() = true;
        IsStartedUp() = false;
    }

    static bool IsRunning() {
        return IsStartedUp() && !IsDestroyed();
    }

protected:
    virtual void onStartUp() {}
    virtual void onShutDown() {}

private:
    static T *&InstanceInternal() {
        static T *inst = nullptr;
        return inst;
    }

    static bool &IsStartedUp() {
        static bool inst = false;
        return inst;
    }

    static bool &IsDestroyed() {
        static bool inst = false;
        return inst;
    }
};