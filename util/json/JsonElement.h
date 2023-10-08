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
    const JsonElement *parentNode = nullptr;
//    vector<JsonElement *> childrenNode;
    bool baseNodeLabel = false;

    bool nullValue = false;

    [[nodiscard]]virtual string dump() const = 0;

    [[nodiscard]]virtual string getTypeName() const = 0;

private:
};

class JsonElementNull : public JsonElement {
public:
    const static string typeName;

    JsonElementNull();

    [[nodiscard]] string dump() const override { return "null"; };

    [[nodiscard]] string getTypeName() const override { return JsonElementNull::typeName; };

private:
};

// ------ string value
class JsonElementString : public JsonElement {
public:
    const static string typeName;

    string value;

    explicit JsonElementString(string &text);

    [[nodiscard]] string dump() const override { return '"' + this->value + '"'; };

    [[nodiscard]] string getTypeName() const override { return JsonElementString::typeName; };
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

    [[nodiscard]] string getTypeName() const override { return JsonElementNumber::typeName; };

private:
};

// ------
//class JsonElementMapPare : public JsonElement {
//public:
//    const static string typeName;
//    JsonElementString *key = nullptr;
//
//    JsonElement *value = nullptr;
//
//    void setKey(JsonElementString *element) { this->key = element; };
//
//    void setValue(JsonElement *element) { this->value = element; };
//
//    [[nodiscard]] string getTypeName() const override { return JsonElementMapPare::typeName; };
//
//    [[nodiscard]] string dump() const override { return this->key->dump() + ":" + this->value->dump(); };
//private:
//};

// ------ element container
class JsonElementMap : public JsonElement {
public:

    const static string typeName;

    unordered_map<string, JsonElement*> childrenNode;

    JsonElement &operator[](string &key) {return *this->childrenNode.at(key);};

//    void addValue(JsonElementMapPare *element) { this->childrenNode.push_back(element); }

    [[nodiscard]] string getTypeName() const override { return JsonElementMap::typeName; };


private:
};

// ------
class JsonElementSequence : public JsonElement {
public:
    const static string typeName;
    vector<JsonElement*> childrenNode;
    string elementTypeName;

    void addValue(JsonElement *element);

    [[nodiscard]] string getTypeName() const override { return JsonElementSequence::typeName; };

private:
};

// ------

#endif //FIRSTPROJECT_JSONELEMENT_H
