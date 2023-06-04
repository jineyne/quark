#include "AIBlackboard.h"

bool AIBlackboard::isSet(String key) {
    return mData.contains(key);
}

AIBlackboard::EEntryType AIBlackboard::getType(String key) {
    return isSet(key) ? mData[key].type : EEntryType::Unknown;
}

void AIBlackboard::clear() {
    mData.reset();
}

void AIBlackboard::setValueAsBool(String key, bool value) {
    mData[key] = { EEntryType::Bool, value };
}

bool AIBlackboard::getValueAsBool(String key) {
    if (!mData.contains(key) || (mData.contains(key) && mData[key].type != EEntryType::Bool)) {
        return false;
    }

    return std::any_cast<bool>(mData[key].data);
}

void AIBlackboard::setValueAsInt(String key, int value) {
    mData[key] = { EEntryType::Int, value };
}

int AIBlackboard::getValueAsInt(String key) {
    if (!mData.contains(key) || (mData.contains(key) && mData[key].type != EEntryType::Int)) {
        return 0;
    }

    return std::any_cast<int>(mData[key].data);
}

void AIBlackboard::setValueAsFloat(String key, float value) {
    mData[key] = { EEntryType::Float, value };
}

float AIBlackboard::getValueAsFloat(String key) {
    if (!mData.contains(key) || (mData.contains(key) && mData[key].type != EEntryType::Float)) {
        return 0;
    }

    return std::any_cast<float>(mData[key].data);
}

void AIBlackboard::setValueAsVector3(String key, Vector3 value) {
    mData[key] = { EEntryType::Vector3, value };
}

Vector3 AIBlackboard::getValueAsVector3(String key) {
    if (!mData.contains(key) || (mData.contains(key) && mData[key].type != EEntryType::Vector3)) {
        return Vector3::ZeroVector;
    }

    return std::any_cast<Vector3>(mData[key].data);
}

void AIBlackboard::setValueAsString(String key, String value) {
    mData[key] = { EEntryType::String, value };
}

String AIBlackboard::getValueAsString(String key) {
    if (!mData.contains(key) || (mData.contains(key) && mData[key].type != EEntryType::String)) {
        return String::Empty;
    }

    return std::any_cast<String>(mData[key].data);
}

void AIBlackboard::setValueAsObject(String key, Object *value) {
    if (value == nullptr) {
        mData.remove(key);
    } else {
        mData[key] = {EEntryType::Object, value};
    }
}

Object *AIBlackboard::getValueAsObject(String key) {
    if (!mData.contains(key) || (mData.contains(key) && mData[key].type != EEntryType::Object)) {
        return nullptr;
    }

    return std::any_cast<Object *>(mData[key].data);
}
