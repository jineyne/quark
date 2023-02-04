#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Reflection/Field.h"

class DLL_EXPORT QStruct : public QField {
private:
    TArray<QField *> mChildProperties;
    TArray<QField *> mChildren;

    // TODO: INTERFACE?
    QStruct *mSuperStruct = nullptr;

    size_t mSize = 0;

protected:
    bool bIsPropertyInitialized = false;

public:
    QStruct(QStruct *super, FString name, size_t size);

public:
    bool isChildOf(QStruct *base) const;

    QStruct *getSuperStruct() const { return mSuperStruct; }
    virtual void setSuperStruct(QStruct *newStruct);

    void addCppProperty(class QProperty *property);
    TArray<QField *> getCppProperties(bool deepSearch = true);

    QField *getCppPropertiesByName(const FString &name, bool deepSearch = true);

    void setSize(size_t size) { mSize = size; }
    const size_t &getSize() const { return mSize; }

private:
    virtual void initProperties();

public:
    DECLARE_CLASS(QStruct, QField, );
    static void StaticRegisterNativeQStruct() {
    }
};
