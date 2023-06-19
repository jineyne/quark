#pragma once

enum { INDEX_NONE = -1 };

template <typename T>
using TFunction = std::function<T>;

template <typename T>
using TSharedPtr = std::shared_ptr<T>;

template <typename T>
using TUniquePtr = std::unique_ptr<T>;

template<typename Type, typename... Args>
TSharedPtr<Type> MakeShared(Args &&... args) {
    return std::make_shared<Type>(std::forward<Args>(args)...);
}

template<typename Type>
TSharedPtr<Type> SharedPtr(Type *data) {
    return TSharedPtr<Type>(data);
}

template<typename Type1, typename Type2>
TSharedPtr<Type2> SharedPtr(TSharedPtr<Type1> right, Type2 *ptr) {
    return TSharedPtr<Type2>(right, ptr);
}

template<typename Type, typename Delete>
TSharedPtr<Type> SharedPtr(Type *data, Delete del = Delete()) {
    return TSharedPtr<Type>(data, std::move(del));
}

template<typename Type>
TUniquePtr<Type> UniquePtr(Type *data) {
    return std::unique_ptr<Type>(data);
}

/** Create a new unique pointer using a custom allocator category. */
template<typename Type, typename... Args>
TUniquePtr<Type> MakeUnique(Args &&... args) {
    Type *rawPtr = q_new<Type>(std::forward<Args>(args)...);
    return UniquePtr<Type>(rawPtr);
}