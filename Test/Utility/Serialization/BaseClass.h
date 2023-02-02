#pragma once

#include "TestUtility.h"
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
    TArray<FString> mArrayParam;

    QPROPERTY()
    TArray<int> mIntArrayParam;

public:
    FBaseClass() {
        //
    }

    QFUNCTION()
    virtual int dump() { return 0; }
};