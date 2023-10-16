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

// ------ base element class
    class JsonElement {
    public:

        //this is a recursion function, always return a p-string on heap
        [[nodiscard]] virtual std::string dump() const = 0;

        virtual JsonElement *getCopy() = 0;


    private:
    };

    class JsonElementNull : public JsonElement {
    public:
        const static std::regex typeReg;

        JsonElementNull() = default;

        JsonElementNull(JsonElementNull &other) = default;

        ~JsonElementNull() = default;

        [[nodiscard]] std::string dump() const override { return "null"; };

        JsonElementNull *getCopy() override {
            auto obj = new JsonElementNull(*this);
            return obj;
        }


    private:

    };

    class JsonElementBool : public JsonElement {
    public:
        const static std::regex typeReg;
        bool value = false;
        JsonElementBool()=default;
        JsonElementBool(JsonElementBool&other)=default;

        explicit JsonElementBool(bool &&value) {
            this->value = value;
        };
        ~JsonElementBool() = default;

        void setValue(bool &val) {
            this->value = val;
        };

        [[nodiscard]] std::string dump() const override { return value ? "true" : "false"; }

        JsonElementBool *getCopy() override {
            return new JsonElementBool(*this);
        }

    };

// ------ string value
    class JsonElementString : public JsonElement {
    public:
        const static std::regex typeReg;
        std::string value;

        explicit JsonElementString(std::string &text) {
            this->value = text;
        };

        explicit JsonElementString(std::string &&text) {
            this->value = text;
        };

        [[nodiscard]] std::string dump() const override { return '"' + this->value + '"'; };

        JsonElementString *getCopy() override {
            return new JsonElementString(*this);
        }

    private:
    };

// ------
    class JsonElementNumber : public JsonElement {
    public:
        const static std::regex typeReg;

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

        [[nodiscard]] std::string dump() const override { return this->value; };

        JsonElementNumber *getCopy() override {
            return new JsonElementNumber(*this);
        }

    private:
    };

// ------ element container
    class JsonElementMap : public JsonElement {
    public:

        std::unordered_map<std::string, JsonElement *> childrenNode;

        JsonElementMap() = default;

        ~JsonElementMap();

        // return a value ptr
        JsonElement *&operator[](std::string &&key) { return this->childrenNode.at(key); };

        // copy and store a value ptr
        void setValue(std::string &&key, JsonElement &value) {
            this->childrenNode[key] = value.getCopy();
        };

        [[nodiscard]] std::string dump() const override;

        JsonElementMap *getCopy() override {
            auto temp = new JsonElementMap(*this);
            for (const auto &pair: this->childrenNode) {
                temp->childrenNode[pair.first] = pair.second->getCopy();
            }
            return temp;
        }

    private:

    };

// ------
    class JsonElementSequence : public JsonElement {
    public:
        std::vector<JsonElement *> childrenNode;

        ~JsonElementSequence();

        JsonElement *operator[](size_t &index) { return this->childrenNode.at(index); };

        void addValue(JsonElement *element) { this->childrenNode.push_back(element); };

        [[nodiscard]] std::string dump() const override;

        JsonElementSequence *getCopy() override {
            auto temp = new JsonElementSequence(*this);
            for (auto pair: this->childrenNode) {
                temp->childrenNode.push_back(pair->getCopy());
            }
            return temp;
        }

    private:
    };
}



// ------

//const JsonElement& createElement(const )

#endif //FIRSTPROJECT_JSONELEMENT_H
