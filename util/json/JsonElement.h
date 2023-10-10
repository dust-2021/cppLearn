#ifndef FIRSTPROJECT_JSONELEMENT_H
#define FIRSTPROJECT_JSONELEMENT_H

#include "string"
#include "vector"
#include "regex"
#include "unordered_map"
#include "string"
#include "auto_ptr.h"

// ------ base element class
class JsonElement {
public:
    const static std::string typeName;
    JsonElement *parentNode = nullptr;

    bool init = false;
    bool baseNodeLabel = true;
    bool nullValue = false;

    JsonElement() = default;

    //this is a recursion function, always return a p-string on heap
    [[nodiscard]]virtual std::string dump() const { return ""; };

    [[nodiscard]]virtual std::string getTypeName() const { return JsonElement::typeName; };

private:
};

class JsonElementNull : public JsonElement {
public:
    const static std::string typeName;
    const static std::regex typeReg;

    JsonElementNull() {
        this->init = true;
        this->baseNodeLabel = true;
    };

    [[nodiscard]] std::string dump() const override { return "null"; };

    [[nodiscard]] std::string getTypeName() const override { return JsonElementNull::typeName; };

private:
};

class JsonElementBool : public JsonElement {
public:
    const static std::string typeName;
    const static std::regex typeReg;

    bool value = false;

    JsonElementBool() : JsonElement() {};

    explicit JsonElementBool(bool &&value) : JsonElement() {
        this->value = value;
        this->init = true;
    };

    void setValue(bool &val) {
        this->value = val;
        this->init = true;
    };

    [[nodiscard]] std::string dump() const override { return value ? "true" : "false"; }

    [[nodiscard]] std::string getTypeName() const override { return JsonElementBool::typeName; }
};

// ------ string value
class JsonElementString : public JsonElement {
public:
    const static std::string typeName;
    const static std::regex typeReg;
    std::string value;

    JsonElementString() = default;

    explicit JsonElementString(std::string &text) {
        this->value = text;
        this->init = true;
    };

    explicit JsonElementString(std::string &&text) {
        this->value = text;
        this->init = true;
    };

    [[nodiscard]] std::string dump() const override { return '"' + this->value + '"'; };

    [[nodiscard]] std::string getTypeName() const override { return JsonElementString::typeName; };
private:
};

// ------
class JsonElementNumber : public JsonElement {
public:
    const static std::string typeName;
    const static std::regex typeReg;
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

    [[nodiscard]] std::string getTypeName() const override { return JsonElementNumber::typeName; };

private:
};

// ------ element container
class JsonElementMap : public JsonElement {
public:

    const static std::string typeName;
    bool baseNodeLabel = false;
    bool init = true;
    std::unordered_map<std::string, std::unique_ptr<JsonElement>> childrenNode;

    // TODO: add class type translate before set value
    void setValue(std::string &&key, std::unique_ptr<JsonElement> &value) {
        this->childrenNode[key] = std::move(value);
        this->childrenNode.at(key)->parentNode = this;
    };

    // return origin object by std::move, delete element which is in the childrenNode
    std::unique_ptr<JsonElement> getElement(std::string &&key) {
        auto temp = std::move(this->childrenNode.at(key));
        this->childrenNode.erase(key);
        return std::move(temp); };

    // return a copy value by given key
    JsonElement &operator[](std::string &&key){return *this->childrenNode.at(key);};

    // return a copy object by given key
    JsonElement getValue(std::string &&key) {return *this->childrenNode.at(key);};

    [[nodiscard]] std::string getTypeName() const override { return JsonElementMap::typeName; };

    [[nodiscard]]std::string dump() const override;

private:
};

// ------
class JsonElementSequence : public JsonElement {
public:
    const static std::string typeName;

    bool baseNodeLabel = false;
    bool init = true;
    std::vector<std::unique_ptr<JsonElement>> childrenNode;
    std::string elementTypeName;

    // TODO: add class type translate before set value
    void addValue(std::unique_ptr<JsonElement> &element) { this->childrenNode.push_back(std::move(element)); };

    JsonElement &operator[](size_t &index) { return *this->childrenNode.at(index); };

    [[nodiscard]] std::string getTypeName() const override { return JsonElementSequence::typeName; };

    [[nodiscard]] std::string dump() const override;

private:
};

// ------

//const JsonElement& createElement(const )

#endif //FIRSTPROJECT_JSONELEMENT_H
