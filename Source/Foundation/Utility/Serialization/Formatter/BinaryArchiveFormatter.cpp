#include "BinaryArchiveFormatter.h"
#include "Reflection/Class.h"
#include "Reflection/Object.h"

BinaryArchiveFormatter::BinaryArchiveFormatter(Archive &archive) : mArchive(archive) {

}

BinaryArchiveFormatter::~BinaryArchiveFormatter() {

}

bool BinaryArchiveFormatter::isLoading() {
    return mArchive.isLoading();
}

bool BinaryArchiveFormatter::isSaving() {
    return mArchive.isSaving();
}

Archive &BinaryArchiveFormatter::getUnderlyingArchive() {
    return mArchive;
}

ArchiveFormatter *BinaryArchiveFormatter::createSubtreeReader() {
    return this;
}

bool BinaryArchiveFormatter::hasDocumentTree() const {
    return false;
}

void BinaryArchiveFormatter::enterRecord() {

}

void BinaryArchiveFormatter::leaveRecord() {
    if (isSaving()) {
        size_t nil = 0;
        mArchive << nil;
    }
}

void BinaryArchiveFormatter::enterField(String &name) {
    mArchive << name;
}

void BinaryArchiveFormatter::leaveField() {
}

bool BinaryArchiveFormatter::tryEnterField(String &name, bool enterWhenWriting) {
    if (mArchive.isSaving() && enterWhenWriting) {
        mArchive << name;
        if (name.empty()) {
            return false;
        }


    } else if (mArchive.isLoading()) {
        mArchive << name;
    }

    return true;
}

void BinaryArchiveFormatter::enterArray(int32_t &elementCounts) {
    mArchive << elementCounts;
}

void BinaryArchiveFormatter::leaveArray() {

}

void BinaryArchiveFormatter::enterArrayElement() {

}

void BinaryArchiveFormatter::leaveArrayElement() {

}

void BinaryArchiveFormatter::enterStream() {

}

void BinaryArchiveFormatter::leaveStream() {

}

void BinaryArchiveFormatter::enterStreamElement() {

}

void BinaryArchiveFormatter::leaveStreamElement() {

}

void BinaryArchiveFormatter::enterMap(int32_t &elementCounts) {
    mArchive << elementCounts;
}

void BinaryArchiveFormatter::leaveMap() {

}

void BinaryArchiveFormatter::enterMapElement(String &name) {

}

void BinaryArchiveFormatter::leaveMapElement() {

}

void BinaryArchiveFormatter::enterAttributedValue() {

}

void BinaryArchiveFormatter::enterAttribute(String &attributeName) {
    mArchive << attributeName;
}

void BinaryArchiveFormatter::enterAttributedValueValue() {

}

void BinaryArchiveFormatter::leaveAttribute() {

}

void BinaryArchiveFormatter::leaveAttributedValue() {

}

bool BinaryArchiveFormatter::tryEnterAttribute(String &attributeName, bool enterWhenWriting) {
    return false;
}

bool BinaryArchiveFormatter::tryEnterAttributedValueValue() {
    return false;
}

void BinaryArchiveFormatter::serialize(uint8_t &value) {
    mArchive << value;
}

void BinaryArchiveFormatter::serialize(uint16_t &value) {
    mArchive << value;
}

void BinaryArchiveFormatter::serialize(uint32_t &value) {
    mArchive << value;
}

void BinaryArchiveFormatter::serialize(uint64_t &value) {
    mArchive << value;
}

void BinaryArchiveFormatter::serialize(int8_t &value) {
    mArchive << value;
}

void BinaryArchiveFormatter::serialize(int16_t &value) {
    mArchive << value;
}

void BinaryArchiveFormatter::serialize(int32_t &value) {
    mArchive << value;
}

void BinaryArchiveFormatter::serialize(int64_t &value) {
    mArchive << value;
}

void BinaryArchiveFormatter::serialize(float &value) {
    mArchive << value;
}

void BinaryArchiveFormatter::serialize(double &value) {
    mArchive << value;
}

void BinaryArchiveFormatter::serialize(bool &value) {
    mArchive << value;
}

void BinaryArchiveFormatter::serialize(String &value) {
    mArchive << value;
}

void BinaryArchiveFormatter::serialize(Object *&value) {
    value->serialize(*this);
}

void BinaryArchiveFormatter::serialize(TArray<uint8_t> &value) {
    mArchive.serialize(value.getData(), value.length());
}

void BinaryArchiveFormatter::serialize(void *data, size_t size) {
    mArchive.serialize(data, size);
}
