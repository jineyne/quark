#pragma once

#include "TestUtility.h"
#include "Reflection/Object.h"
#include "OtherData.g.h"

QCLASS()
class OtherData : public Object {
private:
    QPROPERTY()
    int mFoo;

    QPROPERTY()
    int mBar;

public:
    GENERATED_BODY();

public:
    QFUNCTION()
    int getFoo() const { return mFoo; }

    QFUNCTION()
    void setFoo(int value) { mFoo = value; }

    QFUNCTION()
    int getBar() const { return mBar; }

    QFUNCTION()
    void setBar(int value) { mBar = value; }
};
