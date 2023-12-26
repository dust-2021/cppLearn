#include "elements.h"
#include "cstring"
#include "locale"
#include "codecvt"

// json null 单例对象和对象指针
json::elements::nullType json::elements::nullType::_singleNull = nullType();
json::elements::nullType *json::elements::nullType::singlePtr = &_singleNull;

void *json::elements::nullType::operator new(size_t size) {
    return singlePtr;
}

void json::elements::nullType::operator delete(void *ptr) {
    std::cout << "null type delete" << '\n';
}

std::string json::elements::mapType::dump() {
    std::string res = "{";
    for (auto pair: _value) {
        if (res != "{") {
            res += ',';
        }
        // json对象使用`[]`函数生成 却未赋值生成的无效value
        if (pair.second->type() == 0){
            throw elements::jsonError("undefined values of '" + pair.first + '\'');
        }
        res += '"' + pair.first + "\": " + pair.second->dump();
    }
    return res + '}';
}

json::elements::valueType *&json::elements::mapType::at(std::string &key) {
    auto it = _value.find(key);
    if (it == _value.end()){
        // 赋基类防止未赋值行为
        _value[key] = new valueType();
    }
    return _value[key];
}

std::string json::elements::listType::dump() {
    std::string res = "[";
    for (auto item: _value) {
        if (res != "{") {
            res += ',';
        }
        res += item->dump();
    }
    return res + ']';
}

json::elements::jsonIter json::elements::valueType::begin() {
    throw jsonError("element not support traverse");
}

json::elements::jsonIter json::elements::valueType::end() {
    throw jsonError("element not support traverse");
}

const ::json::json json::Null = ::json::json(nullType::singlePtr, true);


std::wstring json::json::_dump(const char *encode) {
    auto res ='"'+ (*_value)->dump() + '"';
    if (std::strcmp(encode, "utf-8") == 0) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
        return convert.from_bytes(res);
    } else {
        throw elements::jsonError("not supported encoding type.");
    }
}
