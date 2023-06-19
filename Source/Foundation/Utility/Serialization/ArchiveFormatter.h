#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/String.h"

class Archive;

class DLL_EXPORT ArchiveFormatter {
public:
    virtual ~ArchiveFormatter() = default;

    virtual bool isLoading() = 0;
    virtual bool isSaving() = 0;

    virtual Archive& getUnderlyingArchive() = 0;
    virtual ArchiveFormatter* createSubtreeReader() { return this; }

    virtual bool hasDocumentTree() const = 0;

    virtual void enterRecord() = 0;
    virtual void leaveRecord() = 0;
    virtual void enterField(String &name) = 0;
    virtual void leaveField() = 0;
    virtual bool tryEnterField(String &name, bool enterWhenWriting) = 0;

    virtual void enterArray(int32_t& elementCounts) = 0;
    virtual void leaveArray() = 0;
    virtual void enterArrayElement() = 0;
    virtual void leaveArrayElement() = 0;

    virtual void enterStream() = 0;
    virtual void leaveStream() = 0;
    virtual void enterStreamElement() = 0;
    virtual void leaveStreamElement() = 0;

    virtual void enterMap(int32_t& elementCounts) = 0;
    virtual void leaveMap() = 0;
    virtual void enterMapElement(String& name) = 0;
    virtual void leaveMapElement() = 0;

    virtual void enterAttributedValue() = 0;
    virtual void enterAttribute(String &attributeName) = 0;
    virtual void enterAttributedValueValue() = 0;
    virtual void leaveAttribute() = 0;
    virtual void leaveAttributedValue() = 0;
    virtual bool tryEnterAttribute(String &attributeName, bool enterWhenWriting) = 0;
    virtual bool tryEnterAttributedValueValue() = 0;

    virtual void serialize(uint8_t& value) = 0;
    virtual void serialize(uint16_t& value) = 0;
    virtual void serialize(uint32_t& value) = 0;
    virtual void serialize(uint64_t& value) = 0;
    virtual void serialize(int8_t& value) = 0;
    virtual void serialize(int16_t& value) = 0;
    virtual void serialize(int32_t& value) = 0;
    virtual void serialize(int64_t& value) = 0;
    virtual void serialize(float& value) = 0;
    virtual void serialize(double& value) = 0;
    virtual void serialize(bool& value) = 0;
    virtual void serialize(String& value) = 0;
    virtual void serialize(class Object*& value) = 0;
    virtual void serialize(TArray<uint8_t>& value) = 0;
    virtual void serialize(void* data, size_t size) = 0;
};