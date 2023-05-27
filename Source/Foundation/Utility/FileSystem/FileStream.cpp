#include "FileStream.h"

#include "Logging/LogDefines.h"

FileStream::FileStream(const Path &path, EStreamAccessMode accessMode, bool freeOnClose)
        : Stream(accessMode), mPath(path), mFreeOnClose(freeOnClose) {
    initialize();
}

FileStream::FileStream() : Stream(EStreamAccessMode::Read) {}

FileStream::~FileStream() {
    close();
}

void FileStream::initialize() {
    std::ios::openmode mode = std::ios_base::binary;
    std::string raw = TCHAR_TO_ANSI(*mPath.toString());

    if (isReadable()) {
        mode |= std::ios::in;
        mIfStream = q_new<std::ifstream>();
        mIfStream->open(raw, mode);
        mIStream = mIfStream;
    } else if (isWritable()) {
        mode |= std::ios::out;
        mFStream = q_new<std::fstream>();
        mFStream->open(raw, mode);
        mIStream = mFStream;
    }

    if (mIStream->fail()) {
        LOG(LogFileSystem, Warning, TEXT("Unable to open \"%ls\": %ls"), ANSI_TO_TCHAR(raw.c_str()), ANSI_TO_TCHAR(std::strerror(mIStream->rdstate() & std::ios::failbit)));
        return;
    }

    mIStream->seekg(0, std::ios_base::end);
    mSize = static_cast<size_t>(mIStream->tellg());
    mIStream->seekg(0, std::ios_base::beg);
}

size_t FileStream::read(void *buf, std::size_t num) {
    mIStream->read(static_cast<char *>(buf),
                   static_cast<std::streamsize>(num));

    return static_cast<size_t>(mIStream->gcount());
}

size_t FileStream::write(const void *buf, size_t num) {
    size_t written = 0;

    if (isWritable() && mIStream) {
        mFStream->write(static_cast<const char *>(buf),
                        static_cast<std::streamsize>(num));
        written = num;
    }

    return written;
}

String FileStream::readWord() {
    if (!isReadable()) {
        return String::Empty;
    }

    std::string word;
    (*mIfStream) >> word;

    return String(ANSI_TO_TCHAR(word.c_str()));
}

String FileStream::readLine() {
    if (!isReadable()) {
        return String::Empty;
    }

    std::string word;
    std::getline(*mIfStream, word);

    return String(ANSI_TO_TCHAR(word.c_str()));
}

void FileStream::skip(size_t count) {
    mIStream->clear(); // Clear fail status in case eof was set

    if (isWritable()) {
        mFStream->seekp(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
    } else {
        mIStream->seekg(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
    }
}

void FileStream::seek(size_t pos) {
    mIStream->clear(); // Clear fail status in case eof was set

    if (isWritable()) {
        mFStream->seekp(static_cast<std::ifstream::pos_type>(pos), std::ios::beg);
    } else {
        mIStream->seekg(static_cast<std::streamoff>(pos), std::ios::beg);
    }
}

size_t FileStream::tell() const {
    mIStream->clear(); // Clear fail status in case eof was set

    if (isWritable()) {
        return static_cast<size_t>(mFStream->tellp());
    }

    return static_cast<size_t>(mIStream->tellg());
}

bool FileStream::eof() {
    return mIStream->eof();
}

#define SAFE_DELETE(x) if (x) { q_delete(x); }

void FileStream::close() {
    if (mIStream) {
        if (mIfStream) {
            mIfStream->close();
        }

        if (mFStream) {
            mFStream->flush();
            mFStream->close();
        }

        if (mFreeOnClose) {
            SAFE_DELETE(mIStream);
            mIfStream = nullptr;
            mFStream = nullptr;
        }
    }
}
