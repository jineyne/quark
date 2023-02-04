#include "MemoryStream.h"

#include "Logging/LogDefines.h"

class membuf : public std::streambuf {
public:
    membuf(char* p, size_t n) { setg(p, p, p + n); }
};

FMemoryStream::FMemoryStream() : FStream() {}
FMemoryStream::FMemoryStream(size_t capacity, EStreamAccessMode access) : FStream(access) {
    realloc(capacity);
    mCursor = mData;
    mEnd = mCursor + capacity;
}

FMemoryStream::FMemoryStream(void *memory, size_t size, EStreamAccessMode access)
        : FStream(access), mOwnsMemory(false) {
    mData = mCursor = static_cast<uint8_t *>(memory);
    mSize = size;
    mEnd = mData + mSize;
}

FMemoryStream::FMemoryStream(FStream &stream)
        : FStream(stream.getAccessMode()) {
    mSize = stream.size();
    mData = mCursor = new uint8_t [mSize];
    mEnd = mData + stream.read(mData, mSize);

    assert(mEnd >= mCursor);
}

FMemoryStream::~FMemoryStream() {
    close();
}

bool FMemoryStream::isFile() const {
    return false;
}

size_t FMemoryStream::read(void *buf, size_t num) {
    size_t count = num;

    if (mCursor + count > mEnd) {
        count = mEnd - mCursor;
    }

    if (count == 0) {
        return 0;
    }

    assert(count <= num);
    memcpy(buf, mCursor, count);
    mCursor += count;

    return count;
}

size_t FMemoryStream::write(const void *buf, size_t num) {
    size_t written = 0;

    if (isWritable()) {
        written = num;

        size_t usedBytes = (mCursor - mData);
        size_t newSize = usedBytes + num;
        if (newSize > mSize) {
            if (mOwnsMemory) {
                realloc(newSize);
            } else {
                written = mSize - usedBytes;
            }
        }

        if (written == 0) {
            return 0;
        }

        memcpy(mCursor, buf, written);
        mCursor += written;

        mEnd = std::max(mCursor, mEnd);
    }

    return written;
}

FString FMemoryStream::readWord() {
    if (eof()) {
        return FString::Empty;
    }

    std::istringstream ss((char *) mCursor, mEnd - mCursor);
    std::string word;
    ss >> word;

    mCursor += word.length();

    return FString(ANSI_TO_TCHAR(word.c_str()));
}

FString FMemoryStream::readLine() {
    if (eof()) {
        return FString::Empty;
    }

    std::istringstream ss((char *) mCursor, mEnd - mCursor);
    std::string word;
    std::getline(ss, word);

    mCursor += word.length();

    return FString(ANSI_TO_TCHAR(word.c_str()));
}

size_t FMemoryStream::size() const {
    return mSize;
}

void FMemoryStream::skip(size_t count) {
    assert((mCursor + count) <= mEnd);
    mCursor = std::min(mCursor + count, mEnd);
}

void FMemoryStream::seek(size_t pos) {
    assert((mCursor + pos) <= mEnd);
    mCursor = std::min(mData + pos, mEnd);
}

size_t FMemoryStream::tell() const {
    return mCursor - mData;
}

bool FMemoryStream::eof() {
    return mCursor >= mEnd;
}

void FMemoryStream::close() {
    if (mData != nullptr) {
        if (mOwnsMemory) {
            delete[] mData;
        }

        mData = nullptr;
    }
}

void FMemoryStream::realloc(size_t size) {
    if (size != mSize) {
        assert(size > mSize);

        // Note: Eventually add support for custom allocators
        auto *buffer = new uint8_t[size];
        if (mData) {
            mCursor = buffer + (mCursor - mData);
            mEnd = buffer + (mEnd - mData);

            memcpy(buffer, mData, mSize);
            delete[] mData;
        } else {
            mCursor = buffer;
            mEnd = buffer;
        }

        mData = buffer;
        mSize = size;
    }
}