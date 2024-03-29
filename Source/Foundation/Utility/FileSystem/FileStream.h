#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Stream.h"
#include "Path.h"

class DLL_EXPORT FileStream : public Stream {
protected:
    Path mPath;

    std::istream *mIStream = nullptr;
    std::ifstream *mIfStream = nullptr;
    std::fstream *mFStream = nullptr;

    bool mFreeOnClose = true;

public:
    FileStream(const Path &path, EStreamAccessMode accessMode = EStreamAccessMode::Read, bool freeOnClose = true);
    ~FileStream() override;

public:
    void initialize();
    bool isFile() const override { return true; }

    virtual size_t read(void *buf, std::size_t num) override;
    virtual size_t write(const void *buf, size_t num) override;

    String readWord() override;
    String readLine() override;

    void skip(size_t count) override;
    void seek(size_t pos) override;

    size_t tell() const override;
    virtual bool eof() override;
    virtual void close() override;

    std::ifstream &stream() const { return *mIfStream; }
    
protected:
    FileStream();
};