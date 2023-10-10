#include <iostream>
#include "JsonElement.h"

// ------
const std::string JsonElement::typeName = "base";
// ------
const std::string JsonElementNull::typeName = "null";
const std::regex JsonElementNull::typeReg = std::regex(R"(^\s*null\s*$)");
// ------
const std::string JsonElementBool::typeName = "bool";
const std::regex JsonElementBool::typeReg = std::regex(R"((^\s*true\s*$|^\s*false\s*$))");
// ------
const std::string JsonElementString::typeName = "string";
const std::regex JsonElementString::typeReg = std::regex(R"(^\s*".*"\s*$)");
// ------
const std::string JsonElementNumber::typeName = "number";
const std::regex JsonElementNumber::typeReg = std::regex(R"(^\s*\d+(\.\d+)?\s*$)");
// ------
const std::string JsonElementMap::typeName = "map";

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

// ------
const std::string JsonElementSequence::typeName = "sequence";

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
