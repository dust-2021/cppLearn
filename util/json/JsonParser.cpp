
#include "JsonParser.h"
#include "sstream"
#include "stack"

using namespace json::element;
using namespace json::parser;
std::regex Parser::numberReg = std::regex(R"(^-?(\d+)(\.\d+)?$)");
std::vector<char> Parser::ignoreChar = {' ', '\n', '\r', '\t'};
std::vector<char> Parser::endPiece = {'\n', ' ', '\t', '\r', '}', ']', ',', ':'};
std::unordered_map<int, std::vector<char>> Parser::endContainer = {{1, {}},
                                                                   {2, {}},
                                                                   {3, {}},
                                                                   {4, {}},
                                                                   {5, {',', '}'}},
                                                                   {6, {',', ']'}}};

JsonElement *json::parse(std::string &text) {
    auto parser = Parser(text);
    return parser.parse();
};

// 指针前进
void Parser::advance(int num) {
    location += num;
    currentPtr += num;
}

JsonElement *Parser::parse() {

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
        throw JsonException("json: mismatched close char");
    }

    if (result == nullptr) {
        throw JsonException("json: cant parse anything");
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
    throw JsonException("json: no close quote.");
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
    /*
     *
     */
    memoryString.clear();
    switch (*currentPtr) {
        case '{':
            // 进入子map元素
            temp = new JsonElementMap();
            if (currentElement != nullptr) {
                box.value = temp;
                currentElement->parseAdd(box);
            }
            currentElement = temp;
            container.push(temp);
            advance();
            break;
        case '[':
            // 进入子list元素
            temp = new JsonElementSequence();
            if (currentElement != nullptr) {
                box.value = temp;
                currentElement->parseAdd(box);
            }
            currentElement = temp;
            container.push(temp);
            advance();
            break;
        case '}':
            // 退出map元素
            if (currentElement->typeCode() != 5) {
                throw JsonException("json: mismatched close char at " + std::to_string(location));
            }
            container.pop();
            currentElement = container.empty() ? nullptr : container.top();
            advance();
            break;
        case ']':
            // 退出list元素
            if (currentElement->typeCode() != 6) {
                throw JsonException("json: mismatched close char at " + std::to_string(location));
            }
            container.pop();
            currentElement = container.empty() ? nullptr : container.top();
            advance();
            break;
        case '"':
            if (!memoryString.empty()) {
                throw JsonException("json: unexpect \" at " + std::to_string(location));
            }
            advance();
            memoryString = innerQuote();

            // 判断字符出现环境
            switch (currentElement->typeCode()) {
                case 5:
                    // key 为空且后置为:
                    if (checkNextChar() == ':' && box.key.empty()) {
                        advance();
                        box.key = memoryString;
                        break;
                    }
                case 6:
                    if (!box.key.empty() && std::find(endContainer[currentElement->typeCode()].begin(),
                                                      endContainer[currentElement->typeCode()].end(),
                                                      *currentPtr) !=
                                            endContainer[currentElement->typeCode()].end()) {
                        advance();
                        box.value = new JsonElementString(memoryString);
                        currentElement->parseAdd(box);
                    } else {
                        throw JsonException("json: unambiguous");
                    }
                    break;
                default:
                    throw JsonException("json: not excepted \" at " + std::to_string(location));
            }
            break;
        case ',':
            if (checkNextChar(1) == '}' || checkNextChar(1) == ']'){
                throw JsonException("json: cant finish container after a ',' at location " + std::to_string(location));
            }
            advance();
            break;
        default:
            normalParse();
    }
}

void Parser::normalParse() {
    memoryString.clear();
    while (std::find(endPiece.begin(), endPiece.end(), *currentPtr) == endPiece.end()) {
        if (*currentPtr == '\0') {
            break;
        }
        if (memoryString.length() >= 50) {
            throw JsonException("json: too long for a value type at location" + std::to_string(location));
        }
        memoryString += *currentPtr;
        advance();
    }
    // 容器内连续结束符
    if (memoryString.empty() && (checkNextChar(1, true) == '}' || checkNextChar(1, true) == ']')) {
        return;
    }

    if (memoryString == "true" || memoryString == "false") {
        box.value = new JsonElementBool(memoryString == "true");
    } else if (memoryString == "null") {
        box.value = new JsonElementNull();
    } else if (std::regex_match(memoryString, numberReg)) {
        box.value = new JsonElementNumber(memoryString);
    } else {
        throw JsonException("json: cant parse '" + memoryString + "' at location" + std::to_string(location));
    }
    currentElement->parseAdd(box);
}
