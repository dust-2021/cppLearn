#ifndef FIRSTPROJECT_JSONELEMENT_H
#define FIRSTPROJECT_JSONELEMENT_H

#include "string"
#include "vector"
#include "regex"

using namespace std;

// ------ base element class
class JsonElement {
public:
    const JsonElement *parentNode = nullptr;
    vector<JsonElement *> childrenNode;
    bool baseNodeLabel = false;
    bool nullValue = false;

    ~JsonElement();

    [[nodiscard]] string getPrint(int maxPrintLength = 10000) const;
    [[nodiscard]] string dump() const;
private:
    const static string typeName;
};

class JsonElementNull : public JsonElement {
public:
    JsonElementNull();
private:
    const static string typeName;
};

// ------ string value
class JsonElementString : public JsonElement {
public:
    string value;

    explicit JsonElementString(string &text);
private:
    const static string typeName;
};

// ------
class JsonElementNumber : public JsonElement {
public:
    string value;

    explicit JsonElementNumber(string &text);

    [[nodiscard]] int asInt() const;

    [[nodiscard]] long asLong() const;

    [[nodiscard]] float asFloat() const;

    [[nodiscard]] double asDouble() const;

    [[nodiscard]] string getPrint() const;
private:
    const static string typeName;
};

// ------
class JsonElementMapPare : public JsonElement {
public:
    string key;
    JsonElement value;

    JsonElementMapPare(string &key, JsonElement &value);
private:
    const static string typeName;
};

// ------ element container
class JsonElementMap : public JsonElement {
public:
    vector<JsonElementMapPare> value;
    JsonElementMap();

private:
    const static string typeName;
};

// ------
class JsonElementSequence : public JsonElement {
public:
    vector<JsonElement> value;
    JsonElementSequence();
private:
    const static string typeName;
};

// ------



#endif //FIRSTPROJECT_JSONELEMENT_H
