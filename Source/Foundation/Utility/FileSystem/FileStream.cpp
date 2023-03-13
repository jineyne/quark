#include "FileStream.h"

#include "Logging/LogDefines.h"

FFileStream::FFileStream(const FPath &path, EStreamAccessMode accessMode, bool freeOnClose)
        : FStream(accessMode), mPath(path), mFreeOnClose(freeOnClose) {
    initialize();
}

FFileStream::FFileStream() : FStream(EStreamAccessMode::Read) {}

FFileStream::~FFileStream() {
    close();
}

void FFileStream::initialize() {
    std::ios::openmode mode = std::ios_base::binary;
    std::string raw = TCHAR_TO_ANSI(*mPath.toString());

    if (isReadable()) {
        mode |= std::ios::in;
        mIfStream = new std::ifstream();
        mIfStream->open(raw, mode);
        mIStream = mIfStream;
    } else if (isWritable()) {
        mode |= std::ios::out;
        mFStream = new std::fstream();
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

size_t FFileStream::read(void *buf, std::size_t num) {
    mIStream->read(static_cast<char *>(buf),
                   static_cast<std::streamsize>(num));

    return static_cast<size_t>(mIStream->gcount());
}

size_t FFileStream::write(const void *buf, size_t num) {
    size_t written = 0;

    if (isWritable() && mIStream) {
        mFStream->write(static_cast<const char *>(buf),
                        static_cast<std::streamsize>(num));
        written = num;
    }

    return written;
}

FString FFileStream::readWord() {
    if (!isReadable()) {
        return FString::Empty;
    }

    std::string word;
    (*mIfStream) >> word;

    return FString(ANSI_TO_TCHAR(word.c_str()));
}

FString FFileStream::readLine() {
    if (!isReadable()) {
        return FString::Empty;
    }

    std::string word;
    std::getline(*mIfStream, word);

    return FString(ANSI_TO_TCHAR(word.c_str()));
}

void FFileStream::skip(size_t count) {
    mIStream->clear(); // Clear fail status in case eof was set

    if (isWritable()) {
        mFStream->seekp(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
    } else {
        mIStream->seekg(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
    }
}

void FFileStream::seek(size_t pos) {
    mIStream->clear(); // Clear fail status in case eof was set

    if (isWritable()) {
        mFStream->seekp(static_cast<std::ifstream::pos_type>(pos), std::ios::beg);
    } else {
        mIStream->seekg(static_cast<std::streamoff>(pos), std::ios::beg);
    }
}

size_t FFileStream::tell() const {
    mIStream->clear(); // Clear fail status in case eof was set

    if (isWritable()) {
        return static_cast<size_t>(mFStream->tellp());
    }

    return static_cast<size_t>(mIStream->tellg());
}

bool FFileStream::eof() {
    return mIStream->eof();
}

void FFileStream::close() {
    if (mIStream) {
        if (mIfStream) {
            mIfStream->close();
        }

        if (mFStream) {
            mFStream->flush();
            mFStream->close();
        }

        if (mFreeOnClose) {
            mIStream = nullptr;
            mIfStream = nullptr;
            mFStream = nullptr;
        }
    }
}
