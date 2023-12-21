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

        /* 获取容器类型内元素 非容器类直接抛出异常
         * 容器类型 map、 list元素为valueType指针
         * 返回类型 valueType指针的引用，json对象可根据引用直接修改容器内元素值
         * */
        virtual valueType *&at(size_t &index) { throw jsonError("element doesnt support 'at'"); };

        valueType *&at(size_t &&index) { return at(index); };

        virtual valueType *&at(std::string &key) { throw jsonError("element doesnt support 'at'"); };

        valueType *&at(std::string &&key) { return at(key); };

        virtual void push_back(valueType *other) { throw jsonError("element doesnt support 'at'"); };

        virtual jsonIter begin();

        virtual jsonIter end();

        virtual bool asBool() { throw jsonError("element doesnt support 'asBool'"); };

        virtual int asInt() { throw jsonError("element doesnt support 'asInt'"); };

        virtual double asDouble() { throw jsonError("element doesnt support 'asDouble'"); };

        virtual std::string asString() { throw jsonError("element doesnt support 'asString'"); };

        // 是否为空类型
        virtual bool isNull() { return false; };
    private:
    };

    // TODO null类型单例问题
    class nullType : public valueType {
    public:
        int8_t type() override { return 1; };

        bool isNull() override { return true; };

        std::string dump() override { return "null"; };

        // 返回单例指针
        void *operator new(size_t size);

        // 重载取消对单例的释放
        void operator delete(void *ptr);

        nullType() = default;

        valueType *copy_p() override { return singlePtr; }

        static nullType *singlePtr;
    private:
        static nullType _singleNull;
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
        /* 容器类型
         * 元素为堆区valueType指针
         * 析构、dump、复制方法都会递归调用子元素方法
         * */
    public:
        mapType() = default;

        ~mapType() {
            for (const auto &pair: _value) {
                delete pair.second;
            }
        }

        valueType *&at(std::string &key) override { return _value[key]; };

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

        valueType *&at(size_t &index) override { return _value.at(index); }

    private:
        std::vector<valueType *> _value;
    };

    // json迭代器
    class jsonIter {
    public:
        explicit jsonIter(valueType *data) : _data(data) {};

        bool operator!=(const jsonIter &other) { return this->_data != other._data; };

        bool operator==(const jsonIter &other) { return this->_data == other._data; };

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

        template<class init_t>
        explicit json(init_t other) {

            *_value = _generate(other);
        }

        // 传入json值类型初始化json对象 默认为指向原数据的临时json对象
        explicit json(valueType *&&other, bool fake = true) : _value(&other), _fake(fake) {};

        explicit json(valueType *&other, bool fake = true) : _value(&other), _fake(fake) {};

        json(json &other) {
            if (*other._value != nullptr) {
                *_value = (*other._value)->copy_p();
            }
        }

        ~json() {
            // 临时json不释放 _value指向的对象
            if (!_fake) {
                delete *_value;
                *_value = nullptr;
                delete _value;
            }
        }

        /* 重载[]预算
         * 每次[]运算会生成一个临时的json对象，临时对象使用原对象的数据
         * */
        template<class key_t>
        json operator[](key_t &index) {
            if (*_value == nullptr) {
                delete *_value;
                *_value = new mapType();
            }
            return json((*_value)->at(index));
        };

        template<class key_t>
        json operator[](key_t &&index) { return this[index]; };

        // 释放二级指针指向的值 替换二级指针指向的指针的值
        template<class arg_t>
        json &operator=(arg_t arg) {
            delete *_value;
            *_value = _generate(arg);
            return *this;
        }

        //
        template<class arg_t>
        json &operator=(std::initializer_list<arg_t> list) {
            delete *_value;
            *_value = new listType();
            for (auto item: list) {
                (*_value)->push_back(_generate(item));
            }
            return *this;
        };

//        json &operator=(std::initializer_list<std::pair<std::string, int>> list) {
//            _initial_map(list);
//            return *this;
//        }

        json &operator=(std::initializer_list<std::pair<std::string, double>> list) {
            _initial_map(list);
            return *this;
        }

        json &operator=(std::initializer_list<std::pair<std::string, std::string>> list) {
            _initial_map(list);
            return *this;
        }

        jsonIter begin() { return (*_value)->begin(); };

        jsonIter end() { return (*_value)->end(); };

        std::wstring dump(const char *encode="utf-8") ;

        template<class init_t>
        void push_back(init_t other) { (*_value)->push_back(_generate(other)); }

    private:
        valueType **_value = new valueType *;
        json *parent_p = nullptr;
        // 暂时存在的json对象 引用其他对象的值
        bool _fake = false;

        // c++原生类型转为json值类型
        static valueType *_generate(int &arg) { return new numberType(arg); };

        static valueType *_generate(double &arg) { return new numberType(arg); };

        static valueType *_generate(std::string &arg) { return new stringType(arg); }

        static valueType *_generate(const char *arg) { return new stringType(arg); }

        static valueType *_generate(valueType *arg) { return arg->copy_p(); };

        static valueType *_generate(json &arg) { return (*arg._value)->copy_p(); };

        template<class init_t>
        void _initial_map(std::initializer_list<std::pair<std::string, init_t>> list) {
            delete *_value;
            *_value = new mapType();

            for (auto pair: list) {
                (*_value)->at(pair.first) = _generate(pair.second);
            }
        }
    };

    static const ::json::json Null;
}


#endif //FIRSTPROJECT_ELEMENTS_H
