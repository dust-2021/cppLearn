#include <iostream>
#include "JsonElement.h"
#include "stack"

using namespace json;

JsonElement::~JsonElement() = default;

JsonElementMap::JsonElementMap(json::JsonElementMap &other) {
    for (const auto &pair: other.childrenNode) {
        this->childrenNode[pair.first] = pair.second->getCopy();
    }
}

// delete all children element
JsonElementMap::~JsonElementMap() {
    for (auto &pair: this->childrenNode) {
        delete pair.second;
        pair.second = nullptr;
    }
}

std::string JsonElementMap::dump() const {
    std::string result = "{";
    for (auto &iter: this->childrenNode) {
        if (result != "{") {
            result += ',';
        }
        result += '"' + iter.first + '"' + ':' + iter.second->dump();
    }
    result += "}";
    return result;
}

JsonElementMap *JsonElementMap::getCopy() {
    auto temp = new JsonElementMap(*this);
    for (const auto &pair: this->childrenNode) {
        temp->childrenNode[pair.first] = pair.second->getCopy();
    }
    return temp;
}

JsonElementSequence::JsonElementSequence(json::JsonElementSequence &other) {
    for (const auto &item: other.childrenNode) {
        this->childrenNode.push_back(item->getCopy());
    }
}

JsonElementSequence::~JsonElementSequence() {
    for (auto p: this->childrenNode) {
        delete p;
        p = nullptr;
    }
}

std::string JsonElementSequence::dump() const {
    std::string result = "[";
    for (auto &iter: this->childrenNode) {
        if (result != "[") {
            result += ',';
        }
        result += iter->dump();
    }
    result += ']';
    return result;
}

JsonElementSequence *JsonElementSequence::getCopy() {
    auto temp = new JsonElementSequence(*this);
    for (auto pair: this->childrenNode) {
        temp->childrenNode.push_back(pair->getCopy());
    }
    return temp;
}
