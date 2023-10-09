#ifndef FIRSTPROJECT_JSONELEMENT_H
#define FIRSTPROJECT_JSONELEMENT_H

#include "string"
#include "vector"
#include "regex"
#include "unordered_map"

using namespace std;

// ------ base element class
class JsonElement {
public:
    const static string typeName;
    JsonElement *parentNode = nullptr;

    bool init = false;
    bool baseNodeLabel = true;
    bool nullValue = false;

    //this is a recursion function, always return a pstring on heap
    [[nodiscard]]virtual string *dump() const = 0;

    [[nodiscard]]virtual string getTypeName() const = 0;

private:
};

class JsonElementNull : public JsonElement {
public:
    const static string typeName;
    const static regex typeReg;

    JsonElementNull() : JsonElement() {
        this->init = true;
        this->baseNodeLabel = true;
    };

    [[nodiscard]] string *dump() const override { return new string("null"); };

    [[nodiscard]] string getTypeName() const override { return JsonElementNull::typeName; };

private:
};

class JsonElementBool : public JsonElement {
public:
    const static string typeName;
    const static regex typeReg;

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

    [[nodiscard]] string *dump() const override { return new string(value ? "true" : "false"); }

    [[nodiscard]] string getTypeName() const override { return JsonElementBool::typeName; }
};

// ------ string value
class JsonElementString : public JsonElement {
public:
    const static string typeName;
    const static regex typeReg;
    string value;

    JsonElementString() : JsonElement() {};

    explicit JsonElementString(string &text) {
        this->value = text;
        this->init = true;
    };

    [[nodiscard]] string *dump() const override { return new string('"' + this->value + '"'); };

    [[nodiscard]] string getTypeName() const override { return JsonElementString::typeName; };
private:
};

// ------
class JsonElementNumber : public JsonElement {
public:
    const static string typeName;
    const static regex typeReg;
    string value;

    explicit JsonElementNumber(string &text) {
        this->value = text;
        this->init = true;
        this->baseNodeLabel = true;
    };

    [[nodiscard]] int asInt() const { return stoi(this->value); };

    [[nodiscard]] long asLong() const { return stol(this->value); };

    [[nodiscard]] float asFloat() const { return stof(this->value); };

    [[nodiscard]] double asDouble() const { return stod(this->value); };

    [[nodiscard]] string *dump() const override { return new string(this->value); };

    [[nodiscard]] string getTypeName() const override { return JsonElementNumber::typeName; };

private:
};

// ------ element container
class JsonElementMap : public JsonElement {
public:

    const static string typeName;
    bool baseNodeLabel = false;
    bool init = true;
    unordered_map<string, JsonElement *> childrenNode;

    JsonElementMap() : JsonElement() {};

    JsonElement &operator[](string &key) { return *this->childrenNode.at(key); };

    void setValue(string &key, JsonElement &value) { this->childrenNode[key] = &value; };

    [[nodiscard]] string getTypeName() const override { return JsonElementMap::typeName; };

    [[nodiscard]]string *dump() const override;

private:
};

// ------
class JsonElementSequence : public JsonElement {
public:
    const static string typeName;

    bool baseNodeLabel = false;
    bool init = true;
    vector<JsonElement *> childrenNode;
    string elementTypeName;

    JsonElementSequence() : JsonElement() {};

    void addValue(JsonElement *element);

    JsonElement &operator[](int &index) { return *this->childrenNode.at(index); };

    [[nodiscard]] string getTypeName() const override { return JsonElementSequence::typeName; };

private:
};

// ------

#endif //FIRSTPROJECT_JSONELEMENT_H
