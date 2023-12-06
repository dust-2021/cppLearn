#ifndef FIRSTPROJECT_ELEMENTS_H
#define FIRSTPROJECT_ELEMENTS_H

#include "string"
#include "cstdint"
#include "iostream"
#include "vector"
#include "unordered_map"

namespace json::elements {
    class valueType;

    class jsonIter;

    // json 元素异常
    class jsonError : public std::exception {
    public:
        std::string info;

        jsonError() = default;

        explicit jsonError(std::string &&info) : info(info) {};

        explicit jsonError(std::string &info) : info(info) {};

        [[nodiscard]] const char *what() const noexcept override { return this->info.c_str(); };
    };

    class valueType {
    public:

        valueType() = default;

        virtual valueType *copy() { throw jsonError("use base element"); };

        virtual int8_t type() { return 0; };

        virtual std::string dump() { throw jsonError("use base element"); };

        virtual valueType get(size_t index) { throw jsonError("element not support 'get'"); };

        virtual valueType get(size_t &&index) { return get(index); };

        virtual valueType get(std::string &key) { throw jsonError("element not support 'get'"); };

        virtual valueType get(std::string &&key) { return get(key); };

        virtual jsonIter begin();

        virtual jsonIter end();

        virtual bool asBool() { throw jsonError("element not support 'asBool'"); };

        virtual int asInt() { throw jsonError("element not support 'asInt'"); };

        virtual double asDouble() { throw jsonError("element not support 'asDouble'"); };

        virtual bool isNull() { return false; };
    private:
    };

    class nullType : public valueType {
    public:
        int8_t type() override { return 1; };

        bool isNull() override { return true; };

        std::string dump() override { return "null"; };

    private:
    };

    class boolType : public valueType {
    public:
        explicit boolType(bool &&flag) : _value(flag) {};

        explicit boolType(bool &flag) : _value(flag) {};

        int8_t type() override { return 2; }

        std::string dump() override { return _value ? "true" : "false"; };

    private:
        bool _value;
    };

    class stringType : public valueType {
    public:
        explicit stringType(std::string &text) : _value(text) {};

        explicit stringType(std::string &&text) : _value(text) {};

        int8_t type() override { return 3; };

        std::string dump() override { return '"' + _value + '"'; };

    private:
        std::string _value;
    };

    class numberType : public valueType {
    public:
        explicit numberType(std::string &text) : _value(text) {};

        explicit numberType(std::string &&text) : _value(text) {};

        int8_t type() override { return 4; };

        std::string dump() override { return _value; };

        int asInt() override { return std::stoi(_value); };

        double asDouble() override { return std::stod(_value); };

    private:
        std::string _value;
    };

    template<class type_e>
    class mapType : public valueType {
    public:
    private:
        std::unordered_map<std::string, valueType *> _value;
    };

    // json迭代器
    class jsonIter {
    public:
        explicit jsonIter(valueType *data) : _data(data) {};

        bool operator!=(const jsonIter &other) { return this->_data == other._data; };

        jsonIter &operator++() {
            _data += 1;
            return *this;
        };

        valueType &operator*() const { return *_data; };
    private:
        valueType *_data;
    };
}

namespace json {
    using namespace json::elements;

    class json {
        /*
         * json类
         * */
    public:

        json() = default;

        explicit json(valueType &&other) {
            _value = &other;
        }

        explicit json(valueType &other) {
            _value = &other;
        }

        json(json &other) {
            _value = other._value->copy();
        }

        ~json() {
            delete _value;
        }

        // 下标读取
        json operator[](size_t index) {
            return json(_value->get(index));
        };

        // key读取
        json operator[](std::string &index) {
            return json(_value->get(index));
        };

        // 赋值替换
        template<class arg_t>
        json &operator=(std::initializer_list<arg_t> list) {};

        template<class arg_t>
        json &operator=(std::initializer_list<std::initializer_list<arg_t>> list) {};

        jsonIter begin() { return _value->begin(); };

        jsonIter end() { return _value->end(); };


    private:
        valueType *_value = nullptr;
        json *parent_p = nullptr;
    };

}


#endif //FIRSTPROJECT_ELEMENTS_H
