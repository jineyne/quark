#include "YamlArchiveFormatter.h"

#include <yaml-cpp/yaml.h>

#include "Reflection/Class.h"
#include "Reflection/Object.h"
#include "Logging/LogDefines.h"

struct Scope {
    YAML::Node node;
    size_t index;
    YAML::Node::iterator iterator;

    Scope(YAML::Node node) : node(node), index(0), iterator(node.begin()) { }
};

#define GET_CURRENT_SCOPE() mInternal->scope.top()
#define SET_CURRENT_SCOPE auto &scope = GET_CURRENT_SCOPE()

struct YamlArchiveFormatterInternal {
    YAML::Node root;
    TArray<Scope> scope;
};

YamlArchiveFormatter::YamlArchiveFormatter(Archive &archive) : mArchive(archive) {
    mInternal = q_new<YamlArchiveFormatterInternal>();
    if (archive.isSaving()) {
        mInternal->root = YAML::Node();
    } else {
        String data;
        archive << data;
        auto raw = std::string(TCHAR_TO_ANSI(*data));
        mInternal->root = YAML::Load(raw);
    }

    mInternal->scope.add(mInternal->root);
}

YamlArchiveFormatter::~YamlArchiveFormatter() {
    if (isSaving()) {
        YAML::Emitter emitter;
        emitter << mInternal->root;

        auto raw = emitter.c_str();
        String data = ANSI_TO_TCHAR(raw);
        mArchive << data;

        LOG(LogTemp, Debug, TEXT("\n%ls\n"), *data);
    }

    q_delete(mInternal);
}

bool YamlArchiveFormatter::isLoading() {
    return mArchive.isLoading();
}

bool YamlArchiveFormatter::isSaving() {
    return mArchive.isSaving();
}

Archive &YamlArchiveFormatter::getUnderlyingArchive() {
    return mArchive;
}

ArchiveFormatter *YamlArchiveFormatter::createSubtreeReader() {
    return this;
}

bool YamlArchiveFormatter::hasDocumentTree() const {
    return false;
}

void YamlArchiveFormatter::enterRecord() {

}

void YamlArchiveFormatter::leaveRecord() {

}

void YamlArchiveFormatter::enterField(String &name) {
    tryEnterField(name, true);
}

void YamlArchiveFormatter::leaveField() {
    mInternal->scope.pop();
}

bool YamlArchiveFormatter::tryEnterField(String &name, bool enterWhenWriting) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        std::string raw = TCHAR_TO_ANSI(*name);
        mInternal->scope.add(scope.node[raw]);
    } else {
        if (scope.iterator == scope.node.end()) {
            name = String::Empty;
            return false;
        }

        auto it = scope.iterator++;

        std::string key = it->first.as<std::string>();

        name = ANSI_TO_TCHAR(key.data());
        mInternal->scope.add(it->second);
    }

    return true;
}

void YamlArchiveFormatter::enterArray(int32_t &elementCounts) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        for (auto i = 0; i < elementCounts; i++) {
            scope.node.push_back(YAML::Node());
        }
        scope.index = 0;
    } else {
        elementCounts = scope.node.size();
        elementCounts -= 1; // TODO: attribute 뺄 수 있는 방법 찾기
    }
}

void YamlArchiveFormatter::leaveArray() {
    // mInternal->scope.pop();
}

void YamlArchiveFormatter::enterArrayElement() {
    SET_CURRENT_SCOPE;

    mInternal->scope.add(scope.node[scope.index++]);
}

void YamlArchiveFormatter::leaveArrayElement() {
    mInternal->scope.pop();
}

void YamlArchiveFormatter::enterStream() {

}

void YamlArchiveFormatter::leaveStream() {

}

void YamlArchiveFormatter::enterStreamElement() {

}

void YamlArchiveFormatter::leaveStreamElement() {

}

void YamlArchiveFormatter::enterMap(int32_t &elementCounts) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        for (auto i = 0; i < elementCounts; i++) {
            scope.node.push_back(YAML::Node());
        }
        scope.index = 0;
    } else {
        elementCounts = scope.node.size();
        elementCounts -= 2; // TODO: attribute 뺄 수 있는 방법 찾기
    }
}

void YamlArchiveFormatter::leaveMap() {
    mInternal->scope.pop();
}

void YamlArchiveFormatter::enterMapElement(String &name) {
    SET_CURRENT_SCOPE;

    scope.node["key"] = TCHAR_TO_ANSI(*name);
    mInternal->scope.add(scope.node["value"]);
}

void YamlArchiveFormatter::leaveMapElement() {
    mInternal->scope.pop();
}

void YamlArchiveFormatter::enterAttributedValue() {

}

void YamlArchiveFormatter::enterAttribute(String &attributeName) {
    // attribute가 있든 없든 넣는다

    SET_CURRENT_SCOPE;

    std::string raw = TCHAR_TO_ANSI(*attributeName);
    mInternal->scope.add(scope.node[raw]);
}

void YamlArchiveFormatter::enterAttributedValueValue() {

}

void YamlArchiveFormatter::leaveAttribute() {
    mInternal->scope.pop();
}

void YamlArchiveFormatter::leaveAttributedValue() {

}

bool YamlArchiveFormatter::tryEnterAttribute(String &attributeName, bool enterWhenWriting) {
    return false;
}

bool YamlArchiveFormatter::tryEnterAttributedValueValue() {
    return false;
}

void YamlArchiveFormatter::serialize(uint8_t &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.node = value;
    } else {
        value = scope.node.as<uint8_t>();
    }
}

void YamlArchiveFormatter::serialize(uint16_t &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.node = value;
    } else {
        value = scope.node.as<uint16_t>();
    }
}

void YamlArchiveFormatter::serialize(uint32_t &value) {
    SET_CURRENT_SCOPE;
    
    if (isSaving()) {
        scope.node = value;
    } else {
        value = scope.node.as<uint32_t>();
    }
}

void YamlArchiveFormatter::serialize(uint64_t &value) {
    SET_CURRENT_SCOPE;
    
    if (isSaving()) {
        scope.node = value;
    } else {
        value = scope.node.as<uint64_t>();
    }
}

void YamlArchiveFormatter::serialize(int8_t &value) {
    SET_CURRENT_SCOPE;
    
    if (isSaving()) {
        scope.node = value;
    } else {
        value = scope.node.as<int8_t>();
    }
}

void YamlArchiveFormatter::serialize(int16_t &value) {
    SET_CURRENT_SCOPE;
    
    if (isSaving()) {
        scope.node = value;
    } else {
        value = scope.node.as<int16_t>();
    }
}

void YamlArchiveFormatter::serialize(int32_t &value) {
    SET_CURRENT_SCOPE;
    
    if (isSaving()) {
        scope.node = value;
    } else {
        value = scope.node.as<int32_t>();
    }
}

void YamlArchiveFormatter::serialize(int64_t &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.node = value;
    } else {
        value = scope.node.as<int64_t>();
    }
}

void YamlArchiveFormatter::serialize(float &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.node = value;
    } else {
        value = scope.node.as<float>();
    }
}

void YamlArchiveFormatter::serialize(double &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.node = value;
    } else {
        value = scope.node.as<double>();
    }
}

void YamlArchiveFormatter::serialize(bool &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.node = value;
    } else {
        value = scope.node.as<bool>();
    }
}

void YamlArchiveFormatter::serialize(String &value) {
    SET_CURRENT_SCOPE;

    if (isSaving()) {
        scope.node = TCHAR_TO_ANSI(*value);
    } else {
        value = ANSI_TO_TCHAR(scope.node.as<std::string>().data());
    }
}

void YamlArchiveFormatter::serialize(Object *&value) {
    value->serialize(*this);
}

void YamlArchiveFormatter::serialize(TArray<uint8_t> &value) {

}

void YamlArchiveFormatter::serialize(void *data, size_t size) {

}
