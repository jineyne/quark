#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#define FUNC_DECLARE_EVENT(NAME, RETURN, ...) typedef TEvent<RETURN(__VA_ARGS__)> NAME;

template<typename TFunc>
class TEvent;

template<class RetType, class... Args>
class TEvent<RetType(Args ...)> final {
private:
    typedef typename std::function<RetType(Args ...)> Closure;

    struct ComparableClosure {
        Closure callable;
        void *object;
        uint8_t *functor;
        int functorSize;

        ComparableClosure(const ComparableClosure &) = delete;

        ComparableClosure() : object(nullptr), functor(nullptr), functorSize(0) {}

        ComparableClosure(Closure &&closure) : callable(std::move(closure)), object(nullptr), functor(nullptr),
                                               functorSize(0) {
        }

        ~ComparableClosure() {
            if (functor != nullptr) {
                delete[] functor;
            }
        }

        ComparableClosure &operator=(const ComparableClosure &closure) {
            callable = closure.callable;
            object = closure.object;
            functorSize = closure.functorSize;
            if (closure.functorSize == 0) {
                functor = nullptr;
            } else {
                functor = new uint8_t[closure.functorSize];
                std::memcpy(functor, closure.functor, closure.functorSize);
            }

            return *this;
        }

        bool operator==(const ComparableClosure &closure) {
            return object == closure.object && functorSize == closure.functorSize
                   && std::memcmp(functor, closure.functor, functorSize) == 0;
        }
    };

    struct ClosureList {
        ComparableClosure *closures;
        int count;

        ClosureList(ComparableClosure *closures, int count)
                : closures(closures), count(count) {
        }

        ~ClosureList() {
            delete[] closures;
        }
    };

    typedef std::shared_ptr <ClosureList> ClosureListPtr;

private:
    ClosureListPtr mEvents;

private:
    bool addClosure(const ComparableClosure &closure) {
        auto events = std::atomic_load(&mEvents);
        int count;
        ComparableClosure *closures;
        if (events == nullptr) {
            count = 0;
            closures = nullptr;
        } else {
            count = events->count;
            closures = events->closures;
        }

        auto newCount = count + 1;
        auto newClosures = new ComparableClosure[newCount];
        if (count != 0) {
            for (int i = 0; i < count; i++) {
                newClosures[i] = closures[i];
            }
        }

        newClosures[count] = closure;
        auto newEvents = ClosureListPtr(new ClosureList(newClosures, newCount));
        if (std::atomic_compare_exchange_weak(&mEvents, &events, newEvents)) {
            return true;
        }

        return false;
    }

    bool removeClosure(const ComparableClosure &closure) {
        auto events = std::atomic_load(&mEvents);
        if (events == nullptr) {
            return true;
        }

        int index = -1;
        auto count = events->count;
        auto closures = events->closures;
        for (int i = 0; i < count; i++) {
            if (closures[i] == closure) {
                index = i;
                break;
            }
        }

        if (index == -1)
            return true;

        auto newCount = count - 1;
        ClosureListPtr newEvents;
        if (newCount == 0) {
            newEvents = nullptr;
        } else {
            auto newClosures = new ComparableClosure[newCount];
            for (int i = 0; i < index; i++) {
                newClosures[i] = closures[i];
            }

            for (int i = index + 1; i < count; i++) {
                newClosures[i - 1] = closures[i];
            }

            newEvents = ClosureListPtr(new ClosureList(newClosures, newCount));
        }

        if (std::atomic_compare_exchange_weak(&mEvents, &events, newEvents)) {
            return true;
        }

        return false;
    }

public:
    TEvent() {
        std::atomic_store(&mEvents, ClosureListPtr());
    }

    TEvent(const TEvent &event) {
        std::atomic_store(&mEvents, std::atomic_load(&event.mEvents));
    }

    ~TEvent() {
        (*this) = nullptr;
    }

    void operator=(const TEvent &event) {
        std::atomic_store(&mEvents, std::atomic_load(&event.mEvents));
    }

    void operator=(nullptr_t nullpointer) {
        while (true) {
            auto events = std::atomic_load(&mEvents);
            if (!std::atomic_compare_exchange_weak(&mEvents, &events, ClosureListPtr()))
                continue;

            break;
        }
    }

    bool operator==(nullptr_t nullpointer) {
        auto events = std::atomic_load(&mEvents);
        return events == nullptr;
    }

    bool operator!=(nullptr_t nullpointer) {
        auto events = std::atomic_load(&mEvents);
        return events != nullptr;
    }

    void operator+=(Closure f) {
        ComparableClosure closure(std::move(f));
        while (true) {
            if (addClosure(closure)) {
                break;
            }
        }
    }

    void operator-=(Closure f) {
        ComparableClosure closure(std::move(f));
        while (true) {
            if (removeClosure(closure)) {
                break;
            }
        }
    }

    template<typename TObject>
    void bind(RetType(TObject:: *function)(Args...)) {
        ComparableClosure closure;
        closure.callable = function;
        closure.functorSize = sizeof(function);
        closure.functor = new uint8_t[closure.functorSize];
        std::memcpy(closure.functor, (void *)&function, sizeof(function));

        while (true) {
            if (addClosure(closure)) {
                break;
            }
        }
    }

    template<typename TFunc>
    void bindLambda(TFunc function) {
        ComparableClosure closure;
        closure.callable = [function](Args &&...args) {
            function(std::forward<Args>(args)...);
        };
        closure.functorSize = sizeof(function);
        closure.functor = new uint8_t[closure.functorSize];
        std::memcpy(closure.functor, (void *)&function, sizeof(function));

        while (true) {
            if (addClosure(closure)) {
                break;
            }
        }
    }

    template<typename TObject>
    void bind(RetType(TObject:: *function)(Args...), TObject *object) {
        ComparableClosure closure;
        closure.callable = [object, function](Args &&...args) {
            return (object->*function)(std::forward<Args>(args)...);
        };
        closure.functorSize = sizeof(function);
        closure.functor = new uint8_t[closure.functorSize];
        std::memcpy(closure.functor, (void *)&function, sizeof(function));
        closure.object = object;

        while (true) {
            if (addClosure(closure)) {
                break;
            }
        }
    }

    template<typename TObject>
    void unbind(RetType(TObject:: *function)(Args...)) {
        ComparableClosure closure;
        closure.functorSize = sizeof(function);
        closure.functor = new uint8_t[closure.functorSize];
        std::memcpy(closure.functor, (void *)&function, sizeof(function));

        while (true) {
            if (removeClosure(closure)) {
                break;
            }
        }
    }

    template<typename TObject>
    void unbind(RetType(TObject:: *function)(Args...), TObject *object) {
        ComparableClosure closure;
        closure.functorSize = sizeof(function);
        closure.functor = new uint8_t[closure.functorSize];
        std::memcpy(closure.functor, (void *)&function, sizeof(function));
        closure.object = object;

        while (true) {
            if (removeClosure(closure)) {
                break;
            }
        }
    }

    template<typename TFunc>
    void unbindLambda(TFunc function) {
        ComparableClosure closure;
        closure.functorSize = sizeof(function);
        closure.functor = new uint8_t[closure.functorSize];
        std::memcpy(closure.functor, (void *)&function, sizeof(function));

        while (true) {
            if (removeClosure(closure)) {
                break;
            }
        }
    }

    void operator()() {
        auto events = std::atomic_load(&mEvents);
        if (events == nullptr) {
            return;
        }

        auto count = events->count;
        auto closures = events->closures;
        for (int i = 0; i < count; i++) {
            closures[i].callable();
        }
    }

    template<typename TArg0, typename ...Args2>
    void operator()(TArg0 a1, Args2... tail) {
        auto events = std::atomic_load(&mEvents);
        if (events == nullptr)
            return;

        auto count = events->count;
        auto closures = events->closures;
        for (int i = 0; i < count; i++) {
            closures[i].callable(a1, tail...);
        }
    }
};