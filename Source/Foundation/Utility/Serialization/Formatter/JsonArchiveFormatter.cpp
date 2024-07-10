//
// Created by jiney on 2024-06-09.
//

#include "JsonArchiveFormatter.h"

#include "Logging/LogDefines.h"
#include "Reflection/Object.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/error/en.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#define GET_VALUE(X) (*scope.value)[*X]

using UTF16 = rapidjson::UTF16<>;
using Document = rapidjson::GenericDocument<UTF16>;
using Value = rapidjson::GenericValue<UTF16>;
using StringBuffer = rapidjson::GenericStringBuffer<UTF16>;
using PrettyWriter = rapidjson::PrettyWriter<StringBuffer, rapidjson::UTF16<>, rapidjson::UTF16<>>;

struct Scope {
    Value *value;
    size_t index;

    Scope(Value *value) : value(value), index(0) { }
};

#define GET_CURRENT_SCOPE() mInternal->scope.top()
#define SET_CURRENT_SCOPE if (mInternal->scope.empty()) return; auto &scope = GET_CURRENT_SCOPE()
#define SET_CURRENT_SCOPE_B if (mInternal->scope.empty()) return false; auto &scope = GET_CURRENT_SCOPE()

struct JsonArchiveFormatterInternal {
    Document document;
    TArray<Scope> scope;
};

JsonArchiveFormatter::JsonArchiveFormatter(Archive &archive) : mArchive(archive) {
    mInternal = q_new<JsonArchiveFormatterInternal>();

    mInternal->document = {};

    if (!archive.isSaving()) {
        String data;
        archive << data;

        rapidjson::ParseResult ok = mInternal->document.Parse(*data);
        if (!ok) {
            LOG(LogTemp, Error, TEXT("JSON parse error: %s (%u)"), ANSI_TO_TCHAR(rapidjson::GetParseError_En(ok.Code())), ok.Offset());
            return;
        }
    }

    mInternal->scope.add(Scope(nullptr));
}

JsonArchiveFormatter::~JsonArchiveFormatter() {
    if (isSaving()) {
        StringBuffer buffer;
        PrettyWriter writer(buffer);
        mInternal->document.Accept(writer);

        String data = buffer.GetString();
        mArchive << data;

        LOG(LogTemp, Debug, TEXT("\n%ls\n"), *data);
    }

    q_delete(mInternal);
}

bool JsonArchiveFormatter::isLoading() {
    return mArchive.isLoading();
}

bool JsonArchiveFormatter::isSaving() {
    return mArchive.isSaving();
}

Archive &JsonArchiveFormatter::getUnderlyingArchive() {
    return mArchive;
}

ArchiveFormatter *JsonArchiveFormatter::createSubtreeReader() {
    return this;
}

bool JsonArchiveFormatter::hasDocumentTree() const {
    return false;
}

void JsonArchiveFormatter::enterRecord() {

}

void JsonArchiveFormatter::leaveRecord() {

}

void JsonArchiveFormatter::enterField(String &name) {
    tryEnterField(name, true);
}

void JsonArchiveFormatter::leaveField() {
    mInternal->scope.pop();
}

bool JsonArchiveFormatter::tryEnterField(String &name, bool enterWhenWriting) {
    SET_CURRENT_SCOPE_B;

    auto allocator = mInternal->document.GetAllocator();

    if (isSaving()) {
        Value value;
        value.SetObject();

        rapidjson::GenericStringRef<wchar_t> ref = rapidjson::StringRef(*name);
        if (scope.value == nullptr) {
            if (!mInternal->document.IsObject()) {
                mInternal->document.SetObject();
            }

            if (!mInternal->document.HasMember(ref)) {
                mInternal->document.AddMember(ref, value, allocator);
            }

            mInternal->scope.add(Scope(&mInternal->document[*name]));
        } else {
            if (!scope.value->IsObject()) {
                scope.value->SetObject();
            }

            if (!scope.value->HasMember(ref)) {
                scope.value->AddMember(ref, value, allocator);
            }
            mInternal->scope.add(Scope(&(*scope.value)[*name]));
        }
    } else {
        if (scope.value == nullptr) {
            if (mInternal->document.MemberCount() <= scope.index) {
                name = String::Empty;
                return false;
            }

            auto it = mInternal->document.MemberBegin() + scope.index++;
            name = it->name.GetString();
            mInternal->scope.add(Scope(&mInternal->document[*name]));
        } else {
            if (scope.value->MemberCount() <= scope.index) {
                name = String::Empty;
                return false;
            }

            auto it = scope.value->MemberBegin() + scope.index++;
            name = it->name.GetString();
            mInternal->scope.add(Scope(&(*scope.value)[*name]));
        }
        return false;
    }

    return true;
}

void JsonArchiveFormatter::enterArray(int32_t &elementCounts) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        auto a = mInternal->document.GetAllocator();

        scope.value->SetArray();
        for (auto i = 0; i < elementCounts; i++) {
            scope.value->PushBack({}, a);
        }
        scope.index = 0;
    } else {
        if (!scope.value->IsArray()) return;

        elementCounts = scope.value->Size();
    }
}

void JsonArchiveFormatter::leaveArray() {

}

void JsonArchiveFormatter::enterArrayElement() {
    SET_CURRENT_SCOPE;

    mInternal->scope.add(Scope(&(*scope.value)[scope.index++]));
}

void JsonArchiveFormatter::leaveArrayElement() {
    mInternal->scope.pop();
}

void JsonArchiveFormatter::enterStream() {

}

void JsonArchiveFormatter::leaveStream() {

}

void JsonArchiveFormatter::enterStreamElement() {

}

void JsonArchiveFormatter::leaveStreamElement() {

}

void JsonArchiveFormatter::enterMap(int32_t &elementCounts) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        if (!scope.value->IsObject()) {
            scope.value->SetObject();
        }

        for (auto i = 0; i < elementCounts; i++) {
            (*scope.value)[i] = {};
        }

        scope.index = 0;
    } else {
        elementCounts = scope.value->MemberCount();
    }
}

void JsonArchiveFormatter::leaveMap() {
    mInternal->scope.pop();
}

void JsonArchiveFormatter::enterMapElement(String &name) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        mInternal->scope.add(&GET_VALUE(name));
    } else {
        auto it = scope.value->MemberBegin() + scope.index++;
        name = it->name.GetString();
    }
}

void JsonArchiveFormatter::leaveMapElement() {
    mInternal->scope.pop();
}

void JsonArchiveFormatter::enterAttributedValue() {

}

void JsonArchiveFormatter::enterAttribute(String &attributeName) {
    // json 은 attribute가 없음

    attributeName = String::Empty;

    /*SET_CURRENT_SCOPE;

    if (!scope.value->IsObject()) return;

    if (isSaving()) {
        mInternal->scope.add(&GET_VALUE(attributeName));
    } else {
        if (scope.value->MemberCount() < scope.index + 1) return;

        auto it = scope.value->MemberBegin() + scope.index++;
        attributeName = ANSI_TO_TCHAR(it->name.GetString());
    }*/
}

void JsonArchiveFormatter::enterAttributedValueValue() {

}

void JsonArchiveFormatter::leaveAttribute() {
    // mInternal->scope.pop();
}

void JsonArchiveFormatter::leaveAttributedValue() {

}

bool JsonArchiveFormatter::tryEnterAttribute(String &attributeName, bool enterWhenWriting) {
    return false;
}

bool JsonArchiveFormatter::tryEnterAttributedValueValue() {
    return false;
}

void JsonArchiveFormatter::serialize(uint8_t &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.value->SetUint(value);
    } else {
        value = scope.value->GetUint();
    }
}

void JsonArchiveFormatter::serialize(uint16_t &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.value->SetUint(value);
    } else {
        value = scope.value->GetUint();
    }
}

void JsonArchiveFormatter::serialize(uint32_t &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.value->SetUint(value);
    } else {
        value = scope.value->GetUint();
    }
}

void JsonArchiveFormatter::serialize(uint64_t &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.value->SetUint64(value);
    } else {
        value = scope.value->GetUint64();
    }
}

void JsonArchiveFormatter::serialize(int8_t &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.value->SetInt(value);
    } else {
        value = scope.value->GetInt();
    }
}

void JsonArchiveFormatter::serialize(int16_t &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.value->SetInt(value);
    } else {
        value = scope.value->GetInt();
    }
}

void JsonArchiveFormatter::serialize(int32_t &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.value->SetInt(value);
    } else {
        value = scope.value->GetInt();
    }
}

void JsonArchiveFormatter::serialize(int64_t &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.value->SetInt64(value);
    } else {
        value = scope.value->GetInt64();
    }
}

void JsonArchiveFormatter::serialize(float &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.value->SetFloat(value);
    } else {
        value = scope.value->GetFloat();
    }
}

void JsonArchiveFormatter::serialize(double &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.value->SetDouble(value);
    } else {
        value = scope.value->GetDouble();
    }
}

void JsonArchiveFormatter::serialize(bool &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.value->SetBool(value);
    } else {
        value = scope.value->GetBool();
    }
}

void JsonArchiveFormatter::serialize(String &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.value->SetString(*value, value.length());
    } else {
        value = scope.value->GetString();
    }
}

void JsonArchiveFormatter::serialize(Object *&value) {
    value->serialize(*this);
}

void JsonArchiveFormatter::serialize(TArray<uint8_t> &value) {

}

void JsonArchiveFormatter::serialize(void *data, size_t size) {

}

