
#include "JsonParser.h"
#include "sstream"
#include "stack"

using namespace json::element;
using namespace json::parser;
std::regex Parser::numberReg = std::regex(R"(^(\d+)(\.\d+)?)");
std::vector<char> Parser::ignoreChar = {' ', '\n', '\r', '\t'};
std::vector<char> Parser::endPiece = {'\n', '}', ']', ',', ':'};

JsonElement* json::parse(std::string &text) {
  auto parser = Parser(text);
    return parser.parse();
};

JsonElement *Parser::parse() {

    // main loop
    while (*currentPtr != '\0') {
        location++;

        // 跳过空字符
        if (std::find(ignoreChar.begin(), ignoreChar.end(), *currentPtr) != ignoreChar.end()) {
            this->currentPtr++;

            if (!this->memoryString.empty()) {
                afterIgnore = true;
            }
            continue;
        }

        // check char
        if (design && std::find(designChar.begin(), designChar.end(), *currentPtr) == designChar.end()) {
            throw JsonException("json: expect char at " + std::to_string(location));
        } else {
            design = false;
        }

        charSwitch();

        if (result == nullptr) {
            result = currentElement;
        }
        currentPtr++;
    }
    if (!container.empty()) {
        throw JsonException("json: mismatched close char");
    }
    return result;
}

// 解析双引号内内容
std::string Parser::innerQuote(char *&pCurrentChar) {
    std::string result;
    bool escape = false;
    // 字符指针留在结束引号后一位
    while (*pCurrentChar != '\0') {
        pCurrentChar++;

        if (*pCurrentChar == '\\') {
            escape = !escape;
            if (escape) { result += *pCurrentChar; }
            continue;
        }
        if (!escape && *pCurrentChar == '"') {
            pCurrentChar++;
            return result;
        }
        result += *pCurrentChar;
    }
    throw JsonException("json: no close quote.");
}

char Parser::checkNextChar() {
    char *_temp = this->currentPtr;
    _temp++;
    while (std::find(ignoreChar.begin(), ignoreChar.end(), *_temp) == ignoreChar.end()) {
        if (*_temp == '\0') {
            return '\0';
        }
        _temp++;
    }
    return *_temp;
}

void Parser::charSwitch() {
    /*
     *
     */
    memoryString.clear();
    switch (*currentPtr) {

        case '{':
            currentElement = new JsonElementMap();
            break;
        case '[':
            currentElement = new JsonElementSequence();
            break;
        case '}':
            if (currentElement->typeCode() != 5) {
                throw JsonException("json: mismatched close char at " + std::to_string(location));
            }
            currentElement = container.empty() ? container.top() : nullptr;
            container.pop();
            break;
        case ']':
            if (currentElement->typeCode() != 6) {
                throw JsonException("json: mismatched close char at " + std::to_string(location));
            }
            currentElement = container.empty() ? container.top() : nullptr;
            container.pop();
            break;
        case '"':
            if (!memoryString.empty()) {
                throw JsonException("json: unexpect \" at " + std::to_string(location));
            }
            memoryString = innerQuote(currentPtr);
            design = true;
            designChar = {':'};
            break;
        default:;
    }
}


