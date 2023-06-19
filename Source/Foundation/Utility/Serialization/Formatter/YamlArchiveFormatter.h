#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Serialization/ArchiveFormatter.h"

class DLL_EXPORT YamlArchiveFormatter : public ArchiveFormatter {
private:
    Archive &mArchive;
    struct YamlArchiveFormatterInternal *mInternal;

public:
    YamlArchiveFormatter(Archive &archive);
    ~YamlArchiveFormatter() override;

public:
    bool isLoading() override;
    bool isSaving() override;

    Archive &getUnderlyingArchive() override;
    ArchiveFormatter *createSubtreeReader() override;

    bool hasDocumentTree() const override;

    void enterRecord() override;
    void leaveRecord() override;

    void enterField(String &name) override;
    void leaveField() override;
    bool tryEnterField(String &name, bool enterWhenWriting) override;

    void enterArray(int32_t &elementCounts) override;
    void leaveArray() override;

    void enterArrayElement() override;
    void leaveArrayElement() override;

    void enterStream() override;
    void leaveStream() override;

    void enterStreamElement() override;
    void leaveStreamElement() override;

    void enterMap(int32_t &elementCounts) override;
    void leaveMap() override;

    void enterMapElement(String &name) override;
    void leaveMapElement() override;

    void enterAttributedValue() override;
    void enterAttribute(String &attributeName) override;
    void enterAttributedValueValue() override;
    void leaveAttribute() override;
    void leaveAttributedValue() override;
    bool tryEnterAttribute(String &attributeName, bool enterWhenWriting) override;
    bool tryEnterAttributedValueValue() override;

    void serialize(uint8_t &value) override;
    void serialize(uint16_t &value) override;
    void serialize(uint32_t &value) override;
    void serialize(uint64_t &value) override;
    void serialize(int8_t &value) override;
    void serialize(int16_t &value) override;
    void serialize(int32_t &value) override;
    void serialize(int64_t &value) override;
    void serialize(float &value) override;
    void serialize(double &value) override;
    void serialize(bool &value) override;
    void serialize(String &value) override;
    void serialize(Object *&value) override;

    void serialize(TArray<uint8_t> &value) override;
    void serialize(void *data, size_t size) override;
};