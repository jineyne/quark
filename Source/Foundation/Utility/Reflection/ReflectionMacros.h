#pragma once

#define QPROPERTY(...)
#define QFUNCTION(...)
#define QCLASS(...)
#define QSTRUCT(...)
#define QENUM(...)
#define QPARAM(...)
#define QENTRY(...)

#include "ReflectionTypes.h"

#define NO_API

#if REFLECTION_ENABLE

#define DECLARE_CLASS(TClass, TSuperClass, TRequiredAPI) \
    private: \
        TClass& operator=(TClass&&);   \
        TClass& operator=(const TClass&);   \
        TRequiredAPI static Class* GetPrivateStaticClass(); \
    public: \
        /** Typedef for the base class ({{ typedef-type }}) */ \
        using Super = TSuperClass;  \
        /** Typedef for {{ typedef-type }}. */ \
        using ThisClass = TClass;   \
        /** Returns a UClass object representing this class at runtime */ \
        inline static Class* StaticClass() { \
            return GetPrivateStaticClass(); \
        }

#define DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(TClass) \
	static void __DefaultConstructor(void *data) { new (data) TClass(); }

#define IMPLEMENT_CLASS(TClass) \
    Class *TClass::GetPrivateStaticClass() { \
        static Class *instance = nullptr; \
        if (!instance) { \
            Reflection::GetPrivateStaticClass( \
                instance,       \
                &TClass::StaticRegisterNative##TClass, \
                (Class::ClassConstructorType) InternalConstructor<TClass>, \
                sizeof(TClass), \
                TEXT(#TClass),  \
                &TClass::Super::StaticClass   \
            ); \
        } \
        return instance; \
    }

#define IMPLEMENT_CLASS_NO_CTR(TClass) \
    Class *TClass::GetPrivateStaticClass() { \
        static Class *instance = nullptr; \
        if (!instance) { \
            Reflection::GetPrivateStaticClass( \
                instance,       \
                &TClass::StaticRegisterNative##TClass, \
                (Class::ClassConstructorType) nullptr, \
                sizeof(TClass), \
                TEXT(#TClass),  \
                &TClass::Super::StaticClass   \
            ); \
        } \
        return instance; \
    }

#define DECLARE_SERIALIZER(TClass) \
	friend ArchiveFormatter &operator<<(ArchiveFormatter& formatter, TClass* &rhs) { \
		rhs->serialize(formatter); \
		return formatter; \
	}
	/*friend Archive &operator<<(ArchiveFormatter& af, const std::shared_ptr<TClass> &rhs) { \
		rhs->serialize(af); \
		return af; \
	}*/

#else

#define DECLARE_CLASS(TClass, TSuperClass, TRequiredAPI) \
    public:                                              \
        /** Typedef for the base class ({{ typedef-type }}) */ \
        using Super = TSuperClass;  \
        /** Typedef for {{ typedef-type }}. */ \
        using ThisClass = TClass;   \
        inline static Class* StaticClass() { \
            return nullptr; \
        }

#define DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(TClass)
#define IMPLEMENT_CLASS(TClass)
#define DECLARE_SERIALIZER(TClass)
#define IMPLEMENT_CLASS_NO_CTR(TClass)

#endif

#define DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(TClass, TSuperClass, TRequiredAPI) \
    DECLARE_CLASS(TClass, TSuperClass, TRequiredAPI) \
    static void StaticRegisterNative##TClass() {} \
    DECLARE_SERIALIZER(TClass)                                                     \