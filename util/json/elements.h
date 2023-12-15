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

        // 完全复制一个堆区对象
        virtual valueType *copy_p() { throw jsonError("use base element"); };

        // 复制对象
        virtual valueType copy() { throw jsonError("use base element"); };

        // 类型代号
        virtual int8_t type() { return 0; };

        // 生成json dump字符串
        virtual std::string dump() { throw jsonError("use base element"); };

        // 下标获取
        virtual valueType *get(size_t &index) { throw jsonError("element not support 'get'"); };

        valueType *get(size_t &&index) { return get(index); };

        // key获取
        virtual valueType *get(std::string &key) { throw jsonError("element not support 'get'"); };

        valueType *get(std::string &&key) { return get(key); };

        virtual jsonIter begin();

        virtual jsonIter end();

        virtual bool asBool() { throw jsonError("element not support 'asBool'"); };

        virtual int asInt() { throw jsonError("element not support 'asInt'"); };

        virtual double asDouble() { throw jsonError("element not support 'asDouble'"); };

        virtual std::string asString() { throw jsonError("element not support 'asString'"); };

        // 是否为空类型
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

        std::string asString() override { return _value; };

        std::string dump() override { return '"' + _value + '"'; };

    private:
        std::string _value;
    };

    class numberType : public valueType {
    public:
        explicit numberType(std::string &text) : _value(text) {};

        explicit numberType(std::string &&text) : _value(text) {};

        explicit numberType(int &value) : _value(std::to_string(value)) {};

        explicit numberType(double &value) : _value(std::to_string(value)) {};

        int8_t type() override { return 4; };

        std::string dump() override { return _value; };

        int asInt() override { return std::stoi(_value); };

        double asDouble() override { return std::stod(_value); };

    private:
        std::string _value;
    };

    class mapType : public valueType {
    public:
        mapType() = default;

        ~mapType() {
            for (const auto &pair: _value) {
                delete pair.second;
            }
        }

        valueType *get(std::string &key) override { return _value[key]; };

        std::string dump() override;

    private:
        std::unordered_map<std::string, valueType *> _value;
    };

    class listType : public valueType {
    public:
        listType() = default;

        ~listType() {
            for (auto item: _value) {
                delete item;
            }
        }

        std::string dump() override;

        valueType *get(size_t &index) override { return _value.at(index); }

    private:
        std::vector<valueType *> _value;
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

        json() { *_value = nullptr; };

        // 同时生成二级指针指向的对象
        explicit json(int &value) {
            *_value = new numberType(value);
        }

        explicit json(double &value) {
            *_value = new numberType(value);
        }

        json(json &other) {
            if (*other._value != nullptr) {
                *_value = (*other._value)->copy_p();
            }
        }

        ~json() {
            // 临时json不释放 _value指向的对象
            if (!_fake) {
                delete *_value;
            }
            delete _value;
        }

        // 重载[]运算  返回临时json对象
        template<class key_t>
        json operator[](key_t &index) {
            if (*_value == nullptr) {
                delete *_value;
                *_value = new mapType();
            }
            auto ptr = (*_value)->get(index);
            return json(ptr);
        };

        template<class key_t>
        json operator[](key_t &&index) { return this[index]; };

        // 重载赋值
        template<class arg_t>
        json &operator=(arg_t arg) {
            delete *_value;
            *_value = _generate(arg);
            return *this;
        }

        //
        template<class arg_t>
        json &operator=(std::initializer_list<arg_t> list) {
            delete _value;
            *_value = new listType();
            for (auto item: list) {

            }
            return *this;
        };

        //
        template<class arg_t>
        json &operator=(std::initializer_list<std::pair<std::string, arg_t>> list) {
            delete _value;
            *_value = new mapType();

            // TODO 赋值map
            for (auto pair: list) {
                (*_value)->get(pair.first) = _generate(pair.second);
            }
            return *this;
        };

        jsonIter begin() { return (*_value)->begin(); };

        jsonIter end() { return (*_value)->end(); };


    private:
        valueType **_value = new valueType *;
        json *parent_p = nullptr;
        // 暂时存在的json对象 引用其他对象的值
        bool _fake = false;

        // 对象原地址传给临时json 即可根据临时对象修改原json对象内容
        explicit json(valueType *&&other) : _value(&other), _fake(true) {};

        explicit json(valueType *&other) : _value(&other), _fake(true) {};

        static valueType *_generate(int &arg) { return new numberType(arg); };

        static valueType *_generate(double &arg) { return new numberType(arg); };
    };

}


#endif //FIRSTPROJECT_ELEMENTS_H
