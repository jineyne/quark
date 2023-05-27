#pragma once

#include <new>

class LazySingleton {
protected:
    template<class T> static void Construct(void* Place)	{ new (Place) T; }
    template<class T> static void Destruct(T* Instance)		{ Instance->~T(); }
};

template <typename T>
class TLazySingleton final : LazySingleton {
private:
    alignas(T) unsigned char mData[sizeof(T)];
    T *mPtr;

private:
    TLazySingleton(void(*constructor)(void*)) {
        if (constructor) {
            constructor(mData);
        }

        mPtr = constructor ? (T*)mData : nullptr;
    }

public:
    static T &Get() {
        return GetLazy(Construct<T>).GetValue();
    }

    /** Get or create singleton unless it's torn down */
    static T *TryGet() {
        return GetLazy(Construct<T>).TryGetValue();
    }

    /** Destroys singleton. No thread must access the singleton during or after this call. */
    static void TearDown() {
        return GetLazy(nullptr).Reset();
    }
private:
    static TLazySingleton &GetLazy(void(*constructor)(void*)) {
        static TLazySingleton singleton(constructor);
        return singleton;
    }

    T *TryGetValue() {
        return mPtr;
    }

    T &GetValue() {
        assert(mPtr);
        return *mPtr;
    }

    void Reset() {
        if (mPtr) {
            Destruct(mPtr);
            mPtr = nullptr;
        }
    }

};
