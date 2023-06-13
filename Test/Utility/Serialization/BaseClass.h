#pragma once

#include "TestUtility.h"
#include "SaveData.h"
#include "BaseClass.g.h"

QCLASS()
class BaseClass : public Object {
private:
    GENERATED_BODY();

public:
    QPROPERTY()
    int mParam1 = 1;

    QPROPERTY(Server)
    String mParam2 = TEXT("UnInitialized Parameter");

    QPROPERTY()
    TArray<String> mArray;

    QPROPERTY()
    TArray<SaveData> mSaveDataList;

public:
    BaseClass() {
        //
    }

    QFUNCTION()
    virtual int dump() { return 0; }
};
