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
    // didn't use, it's not necessary to set recursion depth.
    static const int8_t MAX_RECURSION_DEPTH = 50;

    class JsonElement;

    // unification object for parse
    struct JsonPiece {
        std::string key;
        std::string value;
        bool flag = false;
        JsonElement *element = nullptr;
    };

// ------ base element class
    class JsonElement {
    public:
        // must be implemented for inherit
        virtual ~JsonElement() = 0;

        [[nodiscard]] virtual std::string dump() const = 0;

        virtual JsonElement *getCopy() = 0;

        virtual void unifySet(JsonPiece &other) = 0;


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

        void unifySet(JsonPiece &other) override {};


    private:

    };

    class JsonElementBool : public JsonElement {
    public:
        bool value = false;

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

        void unifySet(JsonPiece &other) override {
            this->value = other.flag;
        };

    };

// ------ string value
    class JsonElementString : public JsonElement {
    public:
        std::string value;

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

        void unifySet(JsonPiece &other) override {
            this->value = other.value;
        };

    private:
    };

// ------
    class JsonElementNumber : public JsonElement {
    public:

        std::string value;

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

        void unifySet(JsonPiece &other) override {
            this->value = other.value;
        };

    private:
    };

// ------ element container
    class JsonElementMap : public JsonElement {
    public:

        std::unordered_map<std::string, JsonElement *> childrenNode;

        JsonElementMap() = default;

        JsonElementMap(JsonElementMap &other);

        ~JsonElementMap() override;

        [[nodiscard]] std::string dump() const override;

        JsonElementMap *getCopy() override;

        void unifySet(JsonPiece &other) override {
            this->childrenNode[other.key] = other.element->getCopy();
        };

    private:

    };

// ------
    class JsonElementSequence : public JsonElement {
    public:

        std::vector<JsonElement *> childrenNode;

        JsonElementSequence() = default;

        JsonElementSequence(JsonElementSequence &other);

        ~JsonElementSequence() override;

        [[nodiscard]] std::string dump() const override;

        JsonElementSequence *getCopy() override;

        void unifySet(JsonPiece &other) override {
            this->childrenNode.push_back(other.element->getCopy());
        };

    private:
    };
}


#endif //FIRSTPROJECT_JSONELEMENT_H
