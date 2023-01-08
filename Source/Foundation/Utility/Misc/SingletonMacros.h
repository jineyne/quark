#pragma once

#include "LazySingleton.h"

#define DECLARE_SINGLETON(TYPE) \
    /**
     * singleton. return a reference
     */                                             \
    public: static TYPE &Get();                     \
    /**
     * singleton. return null after TearDown() has been called.
     */                                             \
    public: static TYPE *TryGet();                  \
    /**
     *  tear down current singleton
     */                                             \
    public: static void TearDown();                 \
    friend class FLazySingleton;

#define DEFINE_SINGLETON(TYPE)                                         \
    TYPE &TYPE::Get() { return TLazySingleton<TYPE>::Get(); }           \
    TYPE *TYPE::TryGet() { return TLazySingleton<TYPE>::TryGet(); }     \
    void TYPE::TearDown() { TLazySingleton<TYPE>::TearDown(); }
