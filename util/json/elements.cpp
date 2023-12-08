#include "elements.h"

using namespace json;
using namespace json::elements;

std::string mapType::dump() {
    std::string res = "{";
    for (auto pair: _value) {
        if (res != "{") {
            res += ',';
        }
        res += pair.first + ": " + pair.second->dump();
    }
    return res + '}';
}

std::string listType::dump() {
    std::string res = "[";
    for (auto item: _value) {
        if (res != "{") {
            res += ',';
        }
        res += item->dump();
    }
    return res + ']';
}

jsonIter valueType::begin() {
    throw jsonError("element not support traverse");
}

jsonIter valueType::end() {
    throw jsonError("element not support traverse");
}


