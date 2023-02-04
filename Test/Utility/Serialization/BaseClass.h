#pragma once

#include "TestUtility.h"
#include "SaveData.h"
#include "BaseClass.g.h"

QCLASS()
class FBaseClass : public QObject {
private:
    GENERATED_BODY();

public:
    QPROPERTY()
    int mParam1 = 1;

    QPROPERTY(Server)
    FString mParam2 = TEXT("UnInitialized Parameter");

    QPROPERTY()
    TArray<FString> mArray;

    QPROPERTY()
    TArray<FSaveData> mSaveDataList;

public:
    FBaseClass() {
        //
    }

    QFUNCTION()
    virtual int dump() { return 0; }
};