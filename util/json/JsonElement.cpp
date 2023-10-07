#include "JsonElement.h"

// ------
const string JsonElement::typeName = "base";

JsonElement::~JsonElement() {
    if (this->baseNodeLabel) {
        return;
    }
    for (auto p: this->childrenNode) {
        delete p;
    }
}

string JsonElement::getPrint(int maxPrintLength) const { // NOLINT(*-no-recursion)
    string res;
    if (this->baseNodeLabel) {
        res += this->getPrint();
    } else {
        for (JsonElement *j: this->childrenNode) {
            res += j->getPrint();
            // longer than the max print length
            if (res.size() >= maxPrintLength && maxPrintLength != 0) {
                res += "...";
                return res;
            }
        }
    }

    return res;
}

string JsonElement::dump() const {
    return this->getPrint(0);
}

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
const string JsonElementMapPare::typeName = "pare";

JsonElementMapPare::JsonElementMapPare(std::string &key, JsonElement &value) {
    this->key = string(key);
    this->value = JsonElement(value);
    this->childrenNode.push_back(&(this->value));
}

// ------
const string JsonElementMap::typeName = "map";

JsonElementMap::JsonElementMap() {
    this->value = vector<JsonElementMapPare>{};
}

// ------
const string JsonElementSequence::typeName = "sequence";

JsonElementSequence::JsonElementSequence() {
    this->value = vector<JsonElement>{};
}
