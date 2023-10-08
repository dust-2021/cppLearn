#include "JsonElement.h"
#include "JsonParser.h"

// ------
const string JsonElement::typeName = "base";
// ------
const string JsonElementNull::typeName = "null";

JsonElementNull::JsonElementNull() {
    this->nullValue = true;
    this->baseNodeLabel = true;
}

// ------
const string JsonElementString::typeName = "string";

JsonElementString::JsonElementString(string &text) {
    this->baseNodeLabel = true;
    this->value = string(text);
}

// ------
const string JsonElementNumber::typeName = "number";

JsonElementNumber::JsonElementNumber(std::string &text) {
    this->baseNodeLabel = true;
    this->value = string(text);
}

int JsonElementNumber::asInt() const {
    return stoi(this->value);
}

long JsonElementNumber::asLong() const {
    return stol(this->value);
}

float JsonElementNumber::asFloat() const {
    return stof(this->value);
}

double JsonElementNumber::asDouble() const {
    return stod(this->value);
}

string JsonElementNumber::getPrint() const {
    return this->value;
}
// ------
const string JsonElementMap::typeName = "map";

// ------
const string JsonElementSequence::typeName = "sequence";

void JsonElementSequence::addValue(JsonElement *element) {
    if (this->childrenNode.empty()) {
        this->elementTypeName = element->getTypeName();
        this->childrenNode.push_back(element);
        }
    else {
        if (element->getTypeName() != this->elementTypeName){
            throw JsonException("json: bad sequence element type.");
        }
        this->childrenNode.push_back(element);
    }
    }
