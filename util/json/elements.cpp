#include "elements.h"
#include "codecvt"

// json null 单例对象和对象指针
json::elements::nullType json::elements::nullType::_singleNull = nullType();
json::elements::valueType *json::elements::nullType::singlePtr = &_singleNull;
json::inner_type json::elements::nullType::singleDp = &singlePtr;

void *json::elements::nullType::operator new(size_t size) {
    return singlePtr;
}

void json::elements::nullType::operator delete(void *ptr) {
    std::cout << "null type delete" << '\n';
}

std::string json::elements::mapType::dump() {
    std::string res = "{";
    for (const auto& pair: _value) {
        if (res != "{") {
            res += ',';
        }
        // json对象使用`[]`函数生成 却未赋值生成的无效value
        if ((*pair.second)->type() == 0){
            throw elements::jsonError("undefined values of '" + pair.first + '\'');
        }
        res += '"' + pair.first + "\": " + (*pair.second)->dump();
    }
    return res + '}';
}

// 递归复制 temp在栈区 存在栈溢出风险
json::elements::valueType * json::elements::mapType::copy_p() {
    auto temp = new mapType();
    for (const auto& pair: _value){
        temp->_value[pair.first] = new valueType*;
        *temp->_value[pair.first] = (*this->_value[pair.first])->copy_p();
    }
    return temp;
}

json::elements::inner_type json::elements::mapType::at(std::string &key) {
    auto it = _value.find(key);
    if (it == _value.end()){
        _value[key] = new valueType*;
        *_value[key] = new valueType();
    }
    return _value[key];
}

std::string json::elements::listType::dump() {
    std::string res = "[";
    for (auto item: _value) {
        if (res != "[") {
            res += ',';
        }
        res += (*item)->dump();
    }
    return res + ']';
}

// 递归复制 存在栈溢出风险
json::elements::valueType * json::elements::listType::copy_p() {
    auto temp = new listType();
    for (auto item : _value){
        auto i = new valueType*;
        *i = (*item)->copy_p();
        temp->push_back(i);
    }
    return temp;
}

json::elements::jsonIter json::elements::valueType::begin() {
    throw jsonError("element not support traverse");
}

json::elements::jsonIter json::elements::valueType::end() {
    throw jsonError("element not support traverse");
}

const ::json::json json::Null = ::json::json(nullType::singleDp);



