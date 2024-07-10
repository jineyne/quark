#pragma once

#include "Prerequisites/PrerequisitesUtil.h."
#include "Reflection/Reflection.h"

template <typename T>
class IsReflectedClass
{
    typedef char one;
    struct two { char x[2]; };

    template <typename C> static one test( decltype(&C::StaticClass) ) ;
    template <typename C> static two test(...);

public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

class DLL_EXPORT JsonUtility {
public:
    template <typename T>
    static typename std::enable_if<IsReflectedClass<T>::value, String>::type ToJson(T *object) {
        return DumpJsonFromClass((Object *) object);
    }

    template <typename T>
    static typename std::enable_if<!IsReflectedClass<T>::value, String>::type ToJson(T *object) {
        return DumpJsonFromStruct(object, sizeof(T), T::StaticStruct());
    }

    // class
    template <typename T>
    static typename std::enable_if<IsReflectedClass<T>::value, T *>::type FromJson(String json) {
        T *object = newObject<T>();
        ParseJsonFromClass(static_cast<Object *>(object), json);
        return object;
    }

    // struct
    template <typename T>
    static typename std::enable_if<!IsReflectedClass<T>::value, T *>::type FromJson(String json) {
        T *object = q_new<T>();
        ParseJsonFromStruct(object, sizeof(T), T::StaticStruct(), json);
        return object;
    }

private:
    static String DumpJsonFromClass(Object *object);
    static String DumpJsonFromStruct(void *object, size_t size, Struct *s);

    static void ParseJsonFromClass(Object *object, String json);
    static void ParseJsonFromStruct(void *object, size_t size, Struct *s, String json);
};