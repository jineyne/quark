#pragma once

#define QPROPERTY(...)
#define QFUNCTION(...)
#define QCLASS(...)
#define QSTRUCT(...)
#define QENUM(...)
#define QPARAM(...)
#define QENTRY(...)

enum EObjectFlags {
    ObjectFlag_None = 0,
};

enum EStructFlags {
    StructFlags_None = 0,
};

enum EClassFlags {
    ClassFlags_None = 0,
};

enum EPropertyFlags {
    PropertyFlags_None = 0,
    PropertyFlags_AccessBitMask = 0x07,
    PropertyFlags_Public = 0x01,
    PropertyFlags_Protected = 0x02,
    PropertyFlags_Private = 0x04,

    PropertyFlags_NonSerialized = 0x08,
};

#define NO_API

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

#define DECLARE_SERIALIZER(TClass)
	/*friend Archive &operator<<(Archive& ar, TClass* &rhs) { \
		rhs->serialize(ar); \
		return ar; \
	} \
	friend Archive &operator<<(Archive& ar, const std::shared_ptr<TClass> &rhs) { \
		rhs->serialize(ar); \
		return ar; \
	}*/

#define DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(TClass, TSuperClass, TRequiredAPI) \
    DECLARE_CLASS(TClass, TSuperClass, TRequiredAPI) \
    static void StaticRegisterNative##TClass() {} \
    DECLARE_SERIALIZER(TClass) \
