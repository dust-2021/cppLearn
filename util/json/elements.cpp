#include "elements.h"
#include "codecvt"
#include "regex"

using json::parser::Parser;
using json::jsonError;
std::regex Parser::numberReg = std::regex(R"(^-?(\d+)(\.\d+)?$)");
std::vector<char> Parser::ignoreChar = {' ', '\n', '\r', '\t'};
std::vector<char> Parser::endPiece = {'\n', ' ', '\t', '\r', '}', ']', ',', ':'};
std::unordered_map<int, std::vector<char>> Parser::endContainer = {{1, {}},
                                                                   {2, {}},
                                                                   {3, {}},
                                                                   {4, {}},
                                                                   {5, {',', '}'}},
                                                                   {6, {',', ']'}}};

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
            throw jsonError("undefined values of '" + pair.first + '\'');
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
        auto temp = _value[key];
        *temp = new valueType();
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

json::json json::parse(std::string &text) {
    auto parser = Parser(text);
    auto ** temp = new valueType *;
    *temp = parser.parse();
    return json(temp, false);
};

void Parser::advance(int num) {
    location += num;
    currentPtr += num;
}

json::elements::valueType *Parser::parse() {

    // main loop
    while (*currentPtr != '\0') {


        // 跳过空字符
        if (std::find(ignoreChar.begin(), ignoreChar.end(), *currentPtr) != ignoreChar.end()) {
            advance();
            continue;
        }

        // 根据字符判断后续解析行为
        charSwitch();

        if (result == nullptr) {
            result = currentElement;
        }
    }
    if (!container.empty()) {
        throw jsonError("json: mismatched close char");
    }

    if (result == nullptr) {
        throw jsonError("json: cant parse anything");
    }
    return result;
}

std::string Parser::innerQuote() {
    std::string innerString;
    // 字符指针留在结束引号后一位
    while (*currentPtr != '\0') {
        if (*currentPtr == '\\' && checkNextChar(1) == '"') {
            innerString += '"';
            advance(2);
            continue;
        }
        if (*currentPtr == '"') {
            advance();
            return innerString;
        }
        innerString += *currentPtr;
        advance();
    }
    throw jsonError("json: no close quote.");
}

char Parser::checkNextChar(size_t &&offset, bool backStep) const {
    int effectCharNum = 1;
    const char *_temp = this->currentPtr;
    _temp += backStep ? -offset : offset;
    int num = 0;

    nextEffectChar:
    while (std::find(ignoreChar.begin(), ignoreChar.end(), *_temp) != ignoreChar.end()) {
        if (*_temp == '\0') {
            return '\0';
        }
        _temp += backStep ? -1 : 1;
    }
    num += 1;
    if (num == effectCharNum){
        return *_temp;
    } else {
        goto nextEffectChar;
    }
}

void Parser::charSwitch() {
    using elements::valueType;
    /*
     *
     */
    memoryString.clear();
    switch (*currentPtr) {
        case '{':
            // 进入子map元素
            temp = new mapType();
            if (currentElement != nullptr) {
                box.value = new valueType*;
                *box.value = temp;
                currentElement->parseAdd(box);
            }
            currentElement = temp;
            container.push(temp);
            advance();
            break;
        case '[':
            // 进入子list元素
            temp = new listType();
            if (currentElement != nullptr) {
                box.value = new valueType*;
                *box.value = temp;
                currentElement->parseAdd(box);
            }
            currentElement = temp;
            container.push(temp);
            advance();
            break;
        case '}':
            // 退出map元素
            if (currentElement->type() != 5) {
                throw jsonError("json: mismatched close char at " + std::to_string(location));
            }
            container.pop();
            currentElement = container.empty() ? nullptr : container.top();
            advance();
            break;
        case ']':
            // 退出list元素
            if (currentElement->type() != 6) {
                throw jsonError("json: mismatched close char at " + std::to_string(location));
            }
            container.pop();
            currentElement = container.empty() ? nullptr : container.top();
            advance();
            break;
        case '"':
            if (!memoryString.empty()) {
                throw jsonError("json: unexpect \" at " + std::to_string(location));
            }
            advance();
            memoryString = innerQuote();

            // 判断字符出现环境
            switch (currentElement->type()) {
                // 当前为map元素
                case 5:
                    // key 为空且后置为: 将缓存字符串放入box的key 否则进入下个case
                    if (checkNextChar() == ':' && box.key.empty()) {
                        advance();
                        box.key = memoryString;
                        break;
                    }
                case 6:
                    // 下个有效字符为结尾标识符则将缓存字符转为type对象放入box的value， 否则抛出异常
                    if (std::find(endContainer[currentElement->type()].begin(),
                                                      endContainer[currentElement->type()].end(),
                                                     checkNextChar()) !=
                                            endContainer[currentElement->type()].end()) {
//                        advance();
                        box.value = new valueType*;
                        *box.value = new stringType(memoryString);
                        currentElement->parseAdd(box);
                    } else {
                        throw jsonError("json: lost end identifier at " + std::to_string(location));
                    }
                    break;
                default:
                    throw jsonError("json: not excepted \" at " + std::to_string(location));
            }
            break;
        case ',':
            // , 分隔符后不能是容器结束符
            if (checkNextChar(1) == '}' || checkNextChar(1) == ']'){
                throw jsonError("json: cant finish container after a ',' at location " + std::to_string(location));
            }
            advance();
            break;
        default:
            normalParse();
    }
}
// 非容器类型的值解析
void Parser::normalParse() {
    using elements::valueType;
    // 清除缓存字符
    memoryString.clear();
    while (std::find(endPiece.begin(), endPiece.end(), *currentPtr) == endPiece.end()) {
        if (*currentPtr == '\0') {
            break;
        }
        if (memoryString.length() >= 50) {
            throw jsonError("json: too long for a value type at location" + std::to_string(location));
        }
        memoryString += *currentPtr;
        advance();
    }
    // 容器内连续结束符
    if (memoryString.empty() && (checkNextChar(1, true) == '}' || checkNextChar(1, true) == ']')) {
        return;
    }

    if (memoryString == "true" || memoryString == "false") {
        box.value = new valueType*;
        *box.value = new boolType(memoryString == "true");
    } else if (memoryString == "null") {
        box.value = new valueType*;
        *box.value = nullType::singlePtr;
    } else if (std::regex_match(memoryString, numberReg)) {
        box.value = new valueType*;
        *box.value = new numberType(memoryString);
    } else {
        throw jsonError("json: cant parse '" + memoryString + "' at location" + std::to_string(location));
    }
    if (currentElement == nullptr){
        currentElement = *box.value;
        return;
    }
    currentElement->parseAdd(box);
}



