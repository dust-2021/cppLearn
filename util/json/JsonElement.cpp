#include "JsonElement.h"
#include "JsonParser.h"

// ------
const string JsonElement::typeName = "base";
// ------
const string JsonElementNull::typeName = "null";
const regex JsonElementNull::typeReg = regex(R"(^\s*null\s*$)");
// ------
const string JsonElementBool::typeName = "bool";
const regex JsonElementBool::typeReg = regex(R"((^\s*true\s*$|^\s*false\s*$))");
// ------
const string JsonElementString::typeName = "string";
const regex JsonElementString::typeReg = regex(R"(^\s*".*"\s*$)");
// ------
const string JsonElementNumber::typeName = "number";
const regex JsonElementNumber::typeReg = regex (R"(^\s*\d+(\.\d+)?\s*$)");
// ------
const string JsonElementMap::typeName = "map";

string *JsonElementMap::dump() const {
    // create dump string on heap
    auto result = new string();
    for (const auto &iter: this->childrenNode) {
        if (*result != "{") {
            *result += ',';
        }
        auto temp = iter.second->dump();
        *result += *temp;
        delete temp;
    }
    *result += '}';
    return result;
}

// ------
const string JsonElementSequence::typeName = "sequence";

void JsonElementSequence::addValue(JsonElement *element) {
    if (this->childrenNode.empty()) {
        this->elementTypeName = element->getTypeName();
        this->childrenNode.push_back(element);
    } else {
        if (element->getTypeName() != this->elementTypeName) {
            throw JsonException("json: bad sequence element type.");
        }
        this->childrenNode.push_back(element);
    }
}
