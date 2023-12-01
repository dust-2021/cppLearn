#include <iostream>
#include "JsonElement.h"
#include "stack"

using namespace json::element;

JsonElement::~JsonElement() = default;

JsonElementMap::JsonElementMap(JsonElementMap &other) {
    for (const auto &pair: other.childrenNode) {
        this->childrenNode[pair.first] = pair.second->getCopy();
    }
}

// 递归调用子元素析构
JsonElementMap::~JsonElementMap() {
    for (auto &pair: this->childrenNode) {
        delete pair.second;
        pair.second = nullptr;
    }
}

std::string JsonElementMap::_dump() const {
    std::string result = "{";
    for (auto &iter: this->childrenNode) {
        if (result != "{") {
            result += ',';
        }
        result += '"' + iter.first + '"' + ':' + iter.second->_dump();
    }
    result += "}";
    return result;
}

JsonElement* JsonElementMap::operator[](std::string &&key) {
    auto target = this->childrenNode.find(key);
    if (target != this->childrenNode.end()){
        return target->second;
    } else {
        throw ElementException("json: doesnt exist key '" + key + '\'');
    }
}

void JsonElementMap::parseAdd(json::element::JsonPiece &other) {
    this->childrenNode[other.key] = other.value;
    other.clear();
}

JsonElementSequence::JsonElementSequence(JsonElementSequence &other) {
    for (const auto &item: other.childrenNode) {
        this->childrenNode.push_back(item->getCopy());
    }
}

void JsonElementSequence::parseAdd(json::element::JsonPiece &other) {
    this->childrenNode.push_back(other.value);
    other.clear();
}

JsonElementSequence::~JsonElementSequence() {
    for (auto p: this->childrenNode) {
        delete p;
        p = nullptr;
    }
}

std::string JsonElementSequence::_dump() const {
    std::string result = "[";
    for (auto &iter: this->childrenNode) {
        if (result != "[") {
            result += ',';
        }
        result += iter->_dump();
    }
    result += ']';
    return result;
}

