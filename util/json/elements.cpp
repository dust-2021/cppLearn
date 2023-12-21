#include "elements.h"
#include "cstring"
#include "locale"
#include "codecvt"

json::elements::nullType json::elements::nullType::_singleNull = nullType();
json::elements::nullType *json::elements::nullType::singlePtr = &_singleNull;

void *json::elements::nullType::operator new(size_t size) {
    return singlePtr;
}

void json::elements::nullType::operator delete(void *ptr) {}

std::string json::elements::mapType::dump() {
    std::string res = "{";
    for (auto pair: _value) {
        if (res != "{") {
            res += ',';
        }
        res += '"' + pair.first + "\": " + pair.second->dump();
    }
    return res + '}';
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


static const ::json::json Null = ::json::json(json::nullType::singlePtr, true);


std::wstring json::json::dump(const char *encode) {
    auto res ='"'+ (*_value)->dump() + '"';
    if (std::strcmp(encode, "utf-8") == 0) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
        return convert.from_bytes(res);
    } else {
        throw elements::jsonError("not supported encoding type.");
    }
}
