#include <iostream>
#include "JsonElement.h"
#include "JsonParser.h"
#include "stack"
#include "functional"

using namespace json;

const std::regex JsonElementNull::typeReg = std::regex(R"(^\s*null\s*$)");

const std::regex JsonElementBool::typeReg = std::regex(R"((^\s*true\s*$|^\s*false\s*$))");

const std::regex JsonElementString::typeReg = std::regex(R"(^\s*".*"\s*$)");

const std::regex JsonElementNumber::typeReg = std::regex(R"(^\s*\d+(\.\d+)?\s*$)");

// delete all children element
JsonElementMap::~JsonElementMap() {
    for (auto &pair: this->childrenNode) {
        delete pair.second;
        pair.second= nullptr;
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
