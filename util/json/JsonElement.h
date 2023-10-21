#ifndef FIRSTPROJECT_JSONELEMENT_H
#define FIRSTPROJECT_JSONELEMENT_H

#include "string"
#include "vector"
#include "regex"
#include "unordered_map"
#include "iostream"
#include "cstdint"

namespace json::element {
    // haven't used, it's not necessary to set recursion depth.
    static const int8_t MAX_RECURSION_DEPTH = 50;

    class JsonElement;

    // unification object for parse
    struct JsonPiece {
        std::string key;
        std::string value;
        bool flag = false;
        JsonElement *element = nullptr;
        bool active = false;
    };

// ------ base element class
    class JsonElement {

    public:
        // must be implemented for inherit
        virtual ~JsonElement() = 0;

        [[nodiscard]] virtual std::string dump() const = 0;

        virtual JsonElement *getCopy() = 0;

        virtual void unifySetValue(JsonPiece &other) = 0;

        virtual int8_t typeCode() = 0;

    private:
    };

    class JsonElementNull : public JsonElement {

    public:

        JsonElementNull() = default;

        JsonElementNull(JsonElementNull &other) = default;

        ~JsonElementNull() override = default;

        [[nodiscard]] std::string dump() const override { return "null"; };

        JsonElementNull *getCopy() override {
            auto obj = new JsonElementNull(*this);
            return obj;
        }

        void unifySetValue(JsonPiece &other) override {};

        int8_t typeCode() override { return 1; };

    private:

    };

    class JsonElementBool : public JsonElement {
    public:


        JsonElementBool() = default;

        JsonElementBool(JsonElementBool &other) = default;

        explicit JsonElementBool(bool &&value) {
            this->value = value;
        };

        ~JsonElementBool() override = default;

        [[nodiscard]] std::string dump() const override { return value ? "true" : "false"; }

        JsonElementBool *getCopy() override {
            return new JsonElementBool(*this);
        }

        void unifySetValue(JsonPiece &other) override {
            this->value = other.flag;
        };

        int8_t typeCode() override { return 2; };
    private:
        bool value = false;
    };

// ------ string value
    class JsonElementString : public JsonElement {
    public:


        JsonElementString() = default;

        JsonElementString(JsonElementString &other) = default;

        ~JsonElementString() override = default;

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

        void unifySetValue(JsonPiece &other) override {
            this->value = other.value;
        };

        int8_t typeCode() override { return 3; };

    private:
        std::string value;
    };

// ------
    class JsonElementNumber : public JsonElement {
    public:


        JsonElementNumber() = default;

        explicit JsonElementNumber(std::string &text) {
            this->value = text;
        };

        JsonElementNumber(JsonElementNumber &other) = default;

        ~JsonElementNumber() override = default;

        [[nodiscard]] std::string dump() const override { return this->value; };

        JsonElementNumber *getCopy() override {
            return new JsonElementNumber(*this);
        }

        void unifySetValue(JsonPiece &other) override {
            this->value = other.value;
        };

        int8_t typeCode() override { return 4; };

    private:
        std::string value;
    };

// ------ element container
    class JsonElementMap : public JsonElement {
    public:


        JsonElementMap() = default;

        JsonElementMap(JsonElementMap &other);

        ~JsonElementMap() override;

        [[nodiscard]] std::string dump() const override;

        JsonElementMap *getCopy() override;

        void unifySetValue(JsonPiece &other) override {
            this->childrenNode[other.key] = other.element->getCopy();
        };

        int8_t typeCode() override { return 5; };

    private:
        std::unordered_map<std::string, JsonElement *> childrenNode;
    };

// ------
    class JsonElementSequence : public JsonElement {
    public:


        JsonElementSequence() = default;

        JsonElementSequence(JsonElementSequence &other);

        ~JsonElementSequence() override;

        [[nodiscard]] std::string dump() const override;

        JsonElementSequence *getCopy() override;

        void unifySetValue(JsonPiece &other) override {
            this->childrenNode.push_back(other.element->getCopy());
        };

        int8_t typeCode() override { return 6; };
    private:
        std::vector<JsonElement *> childrenNode;
    };

    class ElementException : public std::exception {
    public:
        std::string info;

        ElementException() = default;

        explicit ElementException(std::string &&info) : info(info) {};

        explicit ElementException(std::string &info) : info(info) {};

        [[nodiscard]] const char *what() const noexcept override {
            return this->info.c_str();
        };
    };
}


#endif //FIRSTPROJECT_JSONELEMENT_H
