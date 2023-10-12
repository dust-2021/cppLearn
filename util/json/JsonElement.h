#ifndef FIRSTPROJECT_JSONELEMENT_H
#define FIRSTPROJECT_JSONELEMENT_H

#include "string"
#include "vector"
#include "regex"
#include "unordered_map"
#include "string"
#include "auto_ptr.h"
#include "iostream"
#include "cstdint"

class JsonElement;

class JsonElementNull;

class JsonElementString;

class JsonElementNumber;

class JsonElementMap;

class JsonElementSequence;

static auto clone = [](const auto &p) -> std::decay_t<decltype(*p)> * {
    return new std::decay_t<decltype(*p)>(*p);
};

JsonElement *elementCopy(JsonElement *other);

// ------ base element class
class JsonElement {
public:
    JsonElement *parentNode = nullptr;

    const int8_t typeCode = 0;
    bool init = false;
    bool baseNodeLabel = true;
    bool nullValue = false;

    JsonElement() = default;

    //this is a recursion function, always return a p-string on heap
    [[nodiscard]]virtual std::string dump() const { return ""; };


private:
};

class JsonElementNull : public JsonElement {
public:
    const static std::regex typeReg;

    const int8_t typeCode = 1;

    JsonElementNull() : JsonElement() {
        this->init = true;
        this->baseNodeLabel = true;
    };

    [[nodiscard]] std::string dump() const override { return "null"; };


private:

};

class JsonElementBool : public JsonElement {
public:
    const static std::regex typeReg;

    const int8_t typeCode = 2;
    bool value = false;

    explicit JsonElementBool(bool &&value) : JsonElement() {
        this->value = value;
        this->init = true;
    };

    void setValue(bool &val) {
        this->value = val;
        this->init = true;
    };

    [[nodiscard]] std::string dump() const override { return value ? "true" : "false"; }

};

// ------ string value
class JsonElementString : public JsonElement {
public:
    const static std::regex typeReg;

    const int8_t typeCode = 3;
    std::string value;

    explicit JsonElementString(std::string &text) {
        this->value = text;
        this->init = true;
    };

    explicit JsonElementString(std::string &&text) {
        this->value = text;
        this->init = true;
    };

    [[nodiscard]] std::string dump() const override { return '"' + this->value + '"'; };
private:
};

// ------
class JsonElementNumber : public JsonElement {
public:
    const static std::regex typeReg;

    const int8_t typeCode = 4;
    std::string value;

    explicit JsonElementNumber(std::string &text) {
        this->value = text;
        this->init = true;
        this->baseNodeLabel = true;
    };

    [[nodiscard]] int asInt() const { return stoi(this->value); };

    [[nodiscard]] long asLong() const { return stol(this->value); };

    [[nodiscard]] float asFloat() const { return stof(this->value); };

    [[nodiscard]] double asDouble() const { return stod(this->value); };

    [[nodiscard]] std::string dump() const override { return this->value; };

private:
};

// ------ element container
class JsonElementMap : public JsonElement {
public:

    const int8_t typeCode = 5;
    bool baseNodeLabel = false;
    bool init = true;

    std::unordered_map<std::string, JsonElement *> childrenNode;

    JsonElementMap() = default;

    JsonElementMap(JsonElementMap &other);

    ~JsonElementMap();

    // TODO: add class type translate before set value
    void setValue(std::string &&key, JsonElement *value) {
        this->childrenNode[key] = elementCopy(value);
        this->childrenNode.at(key)->parentNode = this;
    };

    // return a copy value by given key
    JsonElement *&operator[](std::string &&key) { return this->childrenNode.at(key); };

    // return a copy object by given key
    JsonElement getValue(std::string &&key) { return *this->childrenNode.at(key); };

    [[nodiscard]] std::string dump() const override;

private:

};

// ------
class JsonElementSequence : public JsonElement {
public:

    const int8_t typeCode = 6;
    bool baseNodeLabel = false;
    bool init = true;
    std::vector<JsonElement *> childrenNode;

    // TODO: change add method into parse
    void addValue(JsonElement *element) { this->childrenNode.push_back(element); };

    JsonElement &operator[](size_t &index) { return *this->childrenNode.at(index); };


    [[nodiscard]] std::string dump() const override;

private:
};

// ------

//const JsonElement& createElement(const )

#endif //FIRSTPROJECT_JSONELEMENT_H
