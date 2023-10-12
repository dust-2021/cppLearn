#include <iostream>
#include "JsonElement.h"
#include "JsonParser.h"
#include "stack"

JsonElement *elementCopy(JsonElement *other) {
    std:: cout <<other->typeCode <<'\n';
    switch (other->typeCode) {
        case 1: {
            return new JsonElementNull(*dynamic_cast<JsonElementNull *>(other));
        }
        case 2: {
            return new JsonElementBool(*dynamic_cast<JsonElementBool *>(other));
        }
        case 3: {
            return new JsonElementString(*dynamic_cast<JsonElementString *>(other));
        }
        case 4: {
            return new JsonElementNumber(*dynamic_cast<JsonElementNumber *>(other));
        }
        case 5: {
            auto other_ = *dynamic_cast<JsonElementMap *>(other);
            auto new_ele = new JsonElementMap();
            for (const auto &pair: other_.childrenNode) {
                new_ele->childrenNode[pair.first] = elementCopy(pair.second);
            }
            return new_ele;
        }
        case 6: {
            auto other_ = *dynamic_cast<JsonElementSequence *>(other);
            auto new_ele = new JsonElementSequence();
            for (const auto &p: other_.childrenNode) {
                new_ele->childrenNode.push_back(elementCopy(p));
            }
            return new_ele;
        }
        default: {
            return new JsonElement();
        }
    }
}


const std::regex JsonElementNull::typeReg = std::regex(R"(^\s*null\s*$)");

const std::regex JsonElementBool::typeReg = std::regex(R"((^\s*true\s*$|^\s*false\s*$))");

const std::regex JsonElementString::typeReg = std::regex(R"(^\s*".*"\s*$)");

const std::regex JsonElementNumber::typeReg = std::regex(R"(^\s*\d+(\.\d+)?\s*$)");


JsonElementMap::JsonElementMap(JsonElementMap &other) : JsonElement(other) {
    for (const auto &pair: other.childrenNode) {
        this->childrenNode[pair.first] = clone(pair.second);
    }
}

JsonElementMap::~JsonElementMap() {
    for (const auto &pair: this->childrenNode) {
        delete pair.second;
    }
}

std::string JsonElementMap::dump() const {
    // create dump string on heap
    std::string result = "{";
    for (auto &iter: this->childrenNode) {
        if (result != "{") {
            result += ',';
        }
        result += '"' + iter.first + '"' + ':' + iter.second->dump();
    }
    result += '}';
    return result;
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
