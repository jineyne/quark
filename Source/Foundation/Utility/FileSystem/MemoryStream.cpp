#include "MemoryStream.h"

#include "Logging/LogDefines.h"

class membuf : public std::streambuf {
public:
    membuf(char* p, size_t n) { setg(p, p, p + n); }
};

MemoryStream::MemoryStream() : Stream() {}
MemoryStream::MemoryStream(size_t capacity, EStreamAccessMode access) : Stream(access) {
    realloc(capacity);
    mCursor = mData;
    mEnd = mCursor + capacity;
}

MemoryStream::MemoryStream(void *memory, size_t size, EStreamAccessMode access)
        : Stream(access), mOwnsMemory(false) {
    mData = mCursor = static_cast<uint8_t *>(memory);
    mSize = size;
    mEnd = mData + mSize;
}

MemoryStream::MemoryStream(Stream &stream)
        : Stream(stream.getAccessMode()) {
    mSize = stream.size();
    mData = mCursor = q_alloc<uint8_t>(mSize);
    mEnd = mData + stream.read(mData, mSize);

    assert(mEnd >= mCursor);
}

MemoryStream::~MemoryStream() {
    close();
}

bool MemoryStream::isFile() const {
    return false;
}

size_t MemoryStream::read(void *buf, size_t num) {
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

size_t MemoryStream::write(const void *buf, size_t num) {
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

String MemoryStream::readWord() {
    if (eof()) {
        return String::Empty;
    }

    std::istringstream ss((char *) mCursor, mEnd - mCursor);
    std::string word;
    ss >> word;

    mCursor += word.length();

    return String(ANSI_TO_TCHAR(word.c_str()));
}

String MemoryStream::readLine() {
    if (eof()) {
        return String::Empty;
    }

    std::istringstream ss((char *) mCursor, mEnd - mCursor);
    std::string word;
    std::getline(ss, word);

    mCursor += word.length();

    return String(ANSI_TO_TCHAR(word.c_str()));
}

size_t MemoryStream::size() const {
    return mSize;
}

void MemoryStream::skip(size_t count) {
    assert((mCursor + count) <= mEnd);
    mCursor = std::min(mCursor + count, mEnd);
}

void MemoryStream::seek(size_t pos) {
    assert((mCursor + pos) <= mEnd);
    mCursor = std::min(mData + pos, mEnd);
}

size_t MemoryStream::tell() const {
    return mCursor - mData;
}

bool MemoryStream::eof() {
    return mCursor >= mEnd;
}

void MemoryStream::close() {
    if (mData != nullptr) {
        if (mOwnsMemory) {
            q_free(mData);
        }

        mData = nullptr;
    }
}

void MemoryStream::realloc(size_t size) {
    if (size != mSize) {
        assert(size > mSize);

        // Note: Eventually add support for custom allocators
        auto *buffer = q_alloc<uint8_t>(size);
        if (mData) {
            mCursor = buffer + (mCursor - mData);
            mEnd = buffer + (mEnd - mData);

            memcpy(buffer, mData, mSize);
            q_free(mData);
        } else {
            mCursor = buffer;
            mEnd = buffer;
        }

        mData = buffer;
        mSize = size;
    }
}