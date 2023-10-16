#ifndef FIRSTPROJECT_JSONELEMENT_H
#define FIRSTPROJECT_JSONELEMENT_H

#include "string"
#include "vector"
#include "regex"
#include "unordered_map"
#include "string"
#include "iostream"
#include "cstdint"

namespace json {
    class JsonElement;

    class JsonElementNull;

    class JsonElementString;

    class JsonElementNumber;

    class JsonElementMap;

    class JsonElementSequence;

    JsonElement *elementCopy_(JsonElement *other);

    JsonElement *elementCopy(JsonElement *other);

    template<class T>
    T reshape(JsonElement *any);

// ------ base element class
    class JsonElement {
    public:
        JsonElement *parentNode = nullptr;

        const int8_t typeCode = 0;

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
        };

        void setValue(bool &val) {
            this->value = val;
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
        };

        explicit JsonElementString(std::string &&text) {
            this->value = text;
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
        long double int_v;

        explicit JsonElementNumber(std::string &text) {
            this->value = text;
            this->int_v = std::stod(text);
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
        std::vector<JsonElement *> childrenNode;

        ~JsonElementSequence();

        JsonElement *operator[](size_t &index) { return this->childrenNode.at(index); };

        void addValue(JsonElement *element) { this->childrenNode.push_back(element); };

        [[nodiscard]] std::int8_t getType() const override { return this->typeCode; };

        [[nodiscard]] std::string dump() const override;

    private:
    };
}



// ------

//const JsonElement& createElement(const )

#endif //FIRSTPROJECT_JSONELEMENT_H
