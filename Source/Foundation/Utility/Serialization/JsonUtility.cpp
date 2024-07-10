#include "JsonUtility.h"

#include "Reflection/Class.h"
#include "FileSystem/MemoryStream.h"
#include "Serialization/Formatter/JsonArchiveFormatter.h"
#include "Serialization/BinaryArchive.h"

String JsonUtility::DumpJsonFromClass(Object *object) {
    auto stream = MakeShared<MemoryStream>(1024, EStreamAccessMode::Write);
    BinaryArchive sa(stream, EArchiveMode::Save);
    {
        JsonArchiveFormatter formatter(sa);
        formatter.serialize(object);
    }
    String data = *stream->data();

    return data;
}

String JsonUtility::DumpJsonFromStruct(void *object, size_t size, Struct *s) {
    auto stream = MakeShared<MemoryStream>(1024, EStreamAccessMode::Write);
    BinaryArchive sa(stream, EArchiveMode::Save);
    {
        JsonArchiveFormatter formatter(sa);
        s->serialize(object, formatter);
    }
    stream->seek(0);
    String data = *stream->data();

    return data;
}

void JsonUtility::ParseJsonFromClass(Object *object, String json) {
    auto stream = MakeShared<MemoryStream>(json.length(), EStreamAccessMode::Write);

    BinaryArchive sa(stream, EArchiveMode::Save);
    sa << json;

    stream->seek(0);

    BinaryArchive la(stream, EArchiveMode::Load);
    JsonArchiveFormatter formatter(la);

    formatter.serialize(object);
}

void JsonUtility::ParseJsonFromStruct(void *object, size_t size, Struct *s, String json) {
    auto stream = MakeShared<MemoryStream>(json.length(), EStreamAccessMode::Write);

    BinaryArchive sa(stream, EArchiveMode::Save);
    sa << json;

    stream->seek(0);

    BinaryArchive la(stream, EArchiveMode::Load);
    JsonArchiveFormatter formatter(la);

    s->serialize(object, formatter);
}
