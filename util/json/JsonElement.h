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

template<class T>
T *elementTypeCheck(JsonElement *element) {
    return dynamic_cast<T *>(element);
}

JsonElement *elementCopy(JsonElement *other);


// ------ base element class
class JsonElement {
public:
    JsonElement *parentNode = nullptr;

    const int8_t typeCode = 0;
    bool baseNodeLabel = true;

    [[nodiscard]] virtual std::int8_t getType() const { return this->typeCode; };

    //this is a recursion function, always return a p-string on heap
    [[nodiscard]] virtual std::string dump() const { return ""; };

    virtual JsonElement *getCopy() {
        auto obj = new JsonElement(*this);
        return obj;
    };


private:
};

class JsonElementNull : public JsonElement {
public:
    const static std::regex typeReg;

    const int8_t typeCode = 1;
    bool baseNodeLabel= true;

    [[nodiscard]] std::int8_t getType() const override { return this->typeCode; };

    [[nodiscard]] std::string dump() const override { return "null"; };

    JsonElement *getCopy() override {
        auto obj = new JsonElementNull(*this);
        return obj;
    }


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

    [[nodiscard]] std::int8_t getType() const override { return this->typeCode; };

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

    [[nodiscard]] std::int8_t getType() const override { return this->typeCode; };

    [[nodiscard]] std::string dump() const override { return '"' + this->value + '"'; };
private:
};

// ------
class JsonElementNumber : public JsonElement {
public:
    const static std::regex typeReg;

    const int8_t typeCode = 4;

    std::string value;
    int int_v = NULL;

    explicit JsonElementNumber(std::string &text) {
        this->value = text;
        this->init = true;
        this->baseNodeLabel = true;
    };

    [[nodiscard]] int asInt() const { return stoi(this->value); };

    [[nodiscard]] long asLong() const { return stol(this->value); };

    [[nodiscard]] float asFloat() const { return stof(this->value); };

    [[nodiscard]] double asDouble() const { return stod(this->value); };

    [[nodiscard]] std::int8_t getType() const override { return this->typeCode; };

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

    // return a value ptr
    JsonElement *&operator[](std::string &&key) { return this->childrenNode.at(key); };

    // copy and store a value ptr
    void setValue(std::string &&key, JsonElement &value) {
        this->childrenNode[key] = elementCopy(&value);
        this->childrenNode.at(key)->parentNode = this;
    };

    // return a copy object by given key
    JsonElement getValue(std::string &&key) { return *this->childrenNode.at(key); };

    [[nodiscard]] std::int8_t getType() const override { return this->typeCode; };

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

    ~JsonElementSequence();

    JsonElement *operator[](size_t &index) { return this->childrenNode.at(index); };

    void addValue(JsonElement *element) { this->childrenNode.push_back(element); };

    [[nodiscard]] std::int8_t getType() const override { return this->typeCode; };

    [[nodiscard]] std::string dump() const override;

private:
};

// ------

//const JsonElement& createElement(const )

#endif //FIRSTPROJECT_JSONELEMENT_H
