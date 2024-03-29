#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Stream.h"

class DLL_EXPORT MemoryStream : public Stream {
protected:
    uint8_t *mData = nullptr;
    mutable uint8_t *mCursor = nullptr;
    uint8_t *mEnd = nullptr;

    bool mOwnsMemory = true;

public:
    MemoryStream();
    MemoryStream(size_t capacity, EStreamAccessMode access = EStreamAccessMode::Read);
    MemoryStream(void *memory, size_t size, EStreamAccessMode access = EStreamAccessMode::Read);
    MemoryStream(Stream &stream);

    ~MemoryStream();

public:
    uint8_t *data() const { return mData; }
    bool isFile() const override;

    size_t read(void *buf, size_t num) override;
    size_t write(const void *buf, size_t num) override;

    String readWord() override;
    String readLine() override;

    size_t size() const override;
    void skip(size_t count) override;
    void seek(size_t pos) override;
    size_t tell() const override;
    bool eof() override;
    void close() override;

protected:
    void realloc(size_t size);
};