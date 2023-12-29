#ifndef FIRSTPROJECT_ELEMENTS_H
#define FIRSTPROJECT_ELEMENTS_H

#include "string"
#include "cstdint"
#include "iostream"
#include "vector"
#include "unordered_map"

namespace json::elements {
    class valueType;

    // 容器内成员类型 一级指针保证多态 二级指针保证可以直接修改原成员
    typedef valueType **inner_type;

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
        virtual inner_type at(size_t &index) { throw jsonError("element doesnt support 'at'"); };

        inner_type at(size_t &&index) { return at(index); };

        virtual inner_type at(std::string &key) { throw jsonError("element doesnt support 'at'"); };

        inner_type at(std::string &&key) { return at(key); };

        virtual void push_back(inner_type &other) { throw jsonError("element doesnt support 'at'"); };

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

    // 全局单例
    class nullType : public valueType {
    public:
        int8_t type() override { return 1; };

        bool isNull() override { return true; };

        std::string dump() override { return "null"; };

        // 返回单例指针
        static void *operator new(size_t size);

        // 重载取消对单例的释放
        static void operator delete(void *ptr);

        valueType *copy_p() override { return singlePtr; }

        // 单例对象栈指针
        static valueType *singlePtr;

        static inner_type singleDp;
    private:
        nullType() = default;

        ~nullType() = default;

        //单例对象
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

        valueType *copy_p() override { return new stringType(_value); }

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

        valueType *copy_p() override { return new numberType(_value);};

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
                delete *(pair.second);
                delete pair.second;
            }
        }

        // 返回容器内指针引用 可直接修改指针指向的值
        inner_type at(std::string &key) override;

        std::string dump() override;

        valueType * copy_p() override;

        int8_t type() override {return 5;}

    private:
        std::unordered_map<std::string, inner_type> _value;
    };

    class listType : public valueType {
    public:
        listType() = default;

        ~listType() {
            for (auto item: _value) {
                delete *item;
                delete item;
            }
        }

        std::string dump() override;

        void push_back(inner_type &other) override {
            _value.push_back(other);
        }

        inner_type at(size_t &index) override { return _value.at(index); }

        valueType* copy_p() override;

        int8_t type() override {return 6;}

    private:
        std::vector<inner_type> _value;
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

        json(const json &other) {
            *_value = (*other._value)->copy_p();
            _fake = other._fake;
        }

        // 传入json值类型初始化json对象 默认为指向原数据的临时json对象
        explicit json(inner_type &&other, bool fake = true) : _fake(fake) { _value = other; };

        explicit json(inner_type &other, bool fake = true) : _fake(fake) { _value = other; };

        // 拷贝构造拷贝复制体时也生成复制体
        json(json &other) {
            if (*other._value != nullptr) {
                *_value = other._fake ? *(other._value) : (*other._value)->copy_p();
                _fake = other._fake;
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

        // 传入json对象时防止自赋值导致的空指针访问
        json &operator=(const json &other) {
            if (this == &other) { return *this; }
            delete *_value;
            // 传入复制体时this也变为复制体
            *_value = other._fake ? *(other._value) : _generate(other._value);
            _fake = other._fake;
            return *this;
        }

        //
        template<class arg_t>
        json &operator=(std::initializer_list<arg_t> list) {
            delete *_value;
            *_value = new listType();
            for (auto item: list) {
                auto temp = new valueType*;
                *temp = _generate(item);
                (*_value)->push_back(temp);
            }
            return *this;
        };

        //
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

        [[nodiscard]] std::string dump() const { return '"' + (*_value)->dump() + '"'; }

        template<class init_t>
        void push_back(init_t other) { (*_value)->push_back(_generate(other)); }

    private:
        inner_type _value = new valueType *;
        // 暂时存在的json对象 引用其他对象的值
        bool _fake = false;

        // 常用类型转为json值类型
        static valueType *_generate(int &arg) { return new numberType(arg); };

        static valueType *_generate(double &arg) { return new numberType(arg); };

        static valueType *_generate(std::string &arg) { return new stringType(arg); }

        static valueType *_generate(const char *arg) { return new stringType(arg); }

        static valueType *_generate(inner_type arg) { return (*arg)->copy_p(); };

        static valueType *_generate(json &arg) { return (*arg._value)->copy_p(); };

        template<class ele_t>
        static valueType *_generate(std::vector<ele_t> list) {
            auto res = new listType();
            for (auto item: list) {
                res->push_back(_generate(item));
            }
            return res;
        }

        template<class init_t>
        void _initial_map(std::initializer_list<std::pair<std::string, init_t>> list) {
            delete *_value;
            *_value = new mapType();

            for (auto pair: list) {
                *(*_value)->at(pair.first) = _generate(pair.second);
            }
        }
    };

    extern const ::json::json Null;
}


#endif //FIRSTPROJECT_ELEMENTS_H
