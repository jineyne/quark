#include "Class.h"

#include "Reflection/Reflection.h"

IMPLEMENT_CLASS(QClass)

QClass::QClass(size_t size) : QStruct(nullptr, FString::Empty, size) {}