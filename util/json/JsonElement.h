#ifndef FIRSTPROJECT_JSONELEMENT_H
#define FIRSTPROJECT_JSONELEMENT_H

#include "string"
#include "vector"
#include "regex"
#include "unordered_map"
#include "iostream"
#include "cstdint"

namespace json::element {

    // json element exception
    class ElementException : public std::exception {
    public:
        std::string info;

        ElementException() = default;

        explicit ElementException(std::string &&info) : info(info) {};

        explicit ElementException(std::string &info) : info(info) {};

        [[nodiscard]] const char *what() const noexcept override { return this->info.c_str(); };
    };

    // 递归深度
    [[maybe_unused]] static const int8_t MAX_RECURSION_DEPTH = 50;

    class JsonElement;

    // unification object for parse
    struct JsonPiece {
        std::string key;
        JsonElement *value;
    };

    // base element class
    class JsonElement {

    public:
        // must be implemented for inherit
        virtual ~JsonElement() = 0;

        // dump json object to string
        [[nodiscard]] virtual std::string dump() const = 0;

        // return a new json object ptr
        virtual JsonElement *getCopy() = 0;

        // initial object value
        virtual void parseAdd(JsonPiece &other) = 0;

        // return type code
        virtual int8_t typeCode() = 0;

    private:
    };

    // null class
    class JsonElementNull : public JsonElement {

    public:

        JsonElementNull() = default;

        JsonElementNull(JsonElementNull &other) = default;

        ~JsonElementNull() override = default;

        [[nodiscard]] std::string dump() const override { return "null"; };

        JsonElementNull *getCopy() override { return new JsonElementNull(*this); }

        void parseAdd(JsonPiece &other) override { throw ElementException("json: not container"); };

        int8_t typeCode() override { return 1; };

    private:

    };

    // logistic class
    class JsonElementBool : public JsonElement {
    public:


        JsonElementBool() = default;

        JsonElementBool(JsonElementBool &other) = default;

        explicit JsonElementBool(bool &&value) { this->value = value; };

        ~JsonElementBool() override = default;

        [[nodiscard]] std::string dump() const override { return value ? "true" : "false"; }

        JsonElementBool *getCopy() override { return new JsonElementBool(*this); }

        void parseAdd(JsonPiece &other) override { throw ElementException("json: not container"); };

        int8_t typeCode() override { return 2; };
    private:
        bool value = false;
    };

    // string class
    class JsonElementString : public JsonElement {
    public:


        JsonElementString() = default;

        JsonElementString(JsonElementString &other) = default;

        ~JsonElementString() override = default;

        explicit JsonElementString(std::string &text) { this->value = text; };

        explicit JsonElementString(std::string &&text) { this->value = text; };

        [[nodiscard]] std::string dump() const override { return '"' + this->value + '"'; };

        JsonElementString *getCopy() override { return new JsonElementString(*this); }

        void parseAdd(JsonPiece &other) override { throw ElementException("json: not container"); };

        int8_t typeCode() override { return 3; };

    private:
        std::string value;
    };

    // number class
    class JsonElementNumber : public JsonElement {
    public:


        JsonElementNumber() = default;

        explicit JsonElementNumber(std::string &text) { this->value = text; };

        JsonElementNumber(JsonElementNumber &other) = default;

        ~JsonElementNumber() override = default;

        [[nodiscard]] std::string dump() const override { return this->value; };

        JsonElementNumber *getCopy() override { return new JsonElementNumber(*this); }

        void parseAdd(JsonPiece &other) override { throw ElementException("json: not container"); };

        int8_t typeCode() override { return 4; };

    private:
        std::string value;
    };

    // map class
    class JsonElementMap : public JsonElement {
    public:


        JsonElementMap() = default;

        JsonElementMap(JsonElementMap &other);

        ~JsonElementMap() override;

        [[nodiscard]] std::string dump() const override;

        JsonElementMap *getCopy() override { return new JsonElementMap(*this); };

        void parseAdd(JsonPiece &other) override;

        int8_t typeCode() override { return 5; };

        JsonElement *operator[](std::string &key) { return this->childrenNode[key]; }

    private:
        std::unordered_map<std::string, JsonElement *> childrenNode;
    };

    // sequence class
    class JsonElementSequence : public JsonElement {
    public:


        JsonElementSequence() = default;

        JsonElementSequence(JsonElementSequence &other);

        ~JsonElementSequence() override;

        [[nodiscard]] std::string dump() const override;

        JsonElementSequence *getCopy() override { return new JsonElementSequence(*this); };

        void parseAdd(JsonPiece &other) override { this->childrenNode.push_back(other.value); };

        int8_t typeCode() override { return 6; };

        void push_back(JsonElement *other) { this->childrenNode.push_back(other->getCopy()); };
    private:
        std::vector<JsonElement *> childrenNode;
    };
}


#endif //FIRSTPROJECT_JSONELEMENT_H
