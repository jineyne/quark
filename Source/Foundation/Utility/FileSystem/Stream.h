#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Container/String.h"

enum class EStreamAccessMode {
    None,
    Read,
    Write
};

class DLL_EXPORT FStream {
protected:
    FString mName;
    size_t mSize = 0;
    EStreamAccessMode mAccess;

public:
    FStream(EStreamAccessMode access);
    FStream(FString name, EStreamAccessMode access);
    virtual ~FStream() = default;

public:
#define OPERATOR(TYPE) \
    friend FStream &operator<<(FStream &stream, TYPE value) { \
        if (stream.isReadable()) {                            \
            stream.read(&value, sizeof(TYPE));                \
        } else {       \
            stream.write(&value, sizeof(TYPE));               \
        }              \
        return stream; \
    }

#define OPERATOR_SWAP(TYPE) \
    friend FStream &operator<<(FStream &stream, TYPE value) { \
        if (stream.isReadable()) {                            \
            stream.readSwap(&value, sizeof(TYPE));            \
        } else {       \
            stream.writeSwap(&value, sizeof(TYPE));           \
        }                   \
        return stream;      \
    }

    OPERATOR(ANSICHAR&);
    OPERATOR(WIDECHAR&);
    OPERATOR(int8_t &);
    OPERATOR(uint8_t &);

    OPERATOR_SWAP(int16_t &);
    OPERATOR_SWAP(uint16_t &);
    OPERATOR_SWAP(int32_t &);
    OPERATOR_SWAP(uint32_t &);
    OPERATOR_SWAP(int64_t &);
    OPERATOR_SWAP(uint64_t &);

    OPERATOR_SWAP(float &);
    OPERATOR_SWAP(double &);

public:
    EStreamAccessMode getAccessMode() const { return mAccess; }
    virtual bool isReadable() const { return mAccess == EStreamAccessMode::Read; }
    virtual bool isWritable() const { return mAccess == EStreamAccessMode::Write; }
    virtual bool isFile() const = 0;

    virtual size_t read(void *buf, size_t num) = 0;
    virtual size_t write(const void *buf, size_t num) = 0;

    virtual size_t readSwap(void *buf, size_t num);
    virtual size_t writeSwap(void *buf, size_t num);

    virtual FString readWord() = 0;
    virtual FString readLine() = 0;

    virtual size_t size() const { return mSize; }
    virtual void skip(size_t count) = 0;
    virtual void seek(size_t pos) = 0;

    virtual size_t tell() const = 0;
    virtual bool eof() = 0;
    virtual void close() = 0;

    bool isByteSwapping() const;

protected:
    FStream() = default;

protected:
    void byteSwap(void *buf, size_t num);
};
