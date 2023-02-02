#pragma once

#include "TestUtility.h"

#include <Serialization/BinaryArchive.h>
#include <FileSystem/FileSystem.h>
#include <FileSystem/MemoryStream.h>
#include <Reflection/Reflection.h>
#include "BaseClass.h"
#include "ArchiveTest.g.h"

QCLASS()
class FDerivedClass : public FBaseClass {
public:
    GENERATED_BODY();

public:
    QPROPERTY()
    float mFloatValue = 6893.8657;

public:
    QFUNCTION()
    int dump() override;
};