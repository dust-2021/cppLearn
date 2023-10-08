#ifndef FIRSTPROJECT_JSONELEMENT_H
#define FIRSTPROJECT_JSONELEMENT_H

#include "string"
#include "vector"
#include "regex"

using namespace std;

// ------ base element class
class JsonElement {
public:
    const static string typeName;
    const JsonElement *parentNode = nullptr;
    vector<JsonElement *> childrenNode;
    bool baseNodeLabel = false;

    bool nullValue = false;

    [[nodiscard]]virtual string dump() const = 0;

private:
};

class JsonElementNull : public JsonElement {
public:
    const static string typeName;

    JsonElementNull();

    [[nodiscard]]string dump() const override { return "null"; };
private:
};

// ------ string value
class JsonElementString : public JsonElement {
public:
    const static string typeName;

    string value;

    explicit JsonElementString(string &text);

    [[nodiscard]] string dump() const override { return '"' + this->value + '"'; };
private:
};

// ------
class JsonElementNumber : public JsonElement {
public:
    const static string typeName;

    string value;

    explicit JsonElementNumber(string &text);

    [[nodiscard]] int asInt() const;

    [[nodiscard]] long asLong() const;

    [[nodiscard]] float asFloat() const;

    [[nodiscard]] double asDouble() const;

    [[nodiscard]] string getPrint() const;

    [[nodiscard]] string dump() const override { return this->value; };
private:
};

// ------
class JsonElementMapPare : public JsonElement {
public:
    const static string typeName;
    JsonElementString *key = nullptr;

    JsonElement *value = nullptr;

    void setKey(JsonElementString *json) { this->key = json; };

    void setValue(JsonElement *json) { this->value = json; };
private:
};

// ------ element container
class JsonElementMap : public JsonElement {
public:

    const static string typeName;

    void addValue(JsonElementMapPare *element) { this->childrenNode.push_back(element); }

private:
};

// ------
class JsonElementSequence : public JsonElement {
public:
    const static string typeName;

    type_info *typeInfo = nullptr;

    void addValue(JsonElement *element);

private:
};

// ------

#endif //FIRSTPROJECT_JSONELEMENT_H
