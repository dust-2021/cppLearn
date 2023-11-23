
#include "JsonParser.h"
#include "sstream"
#include "stack"

using namespace json::element;
using namespace json::parser;
std::regex Parser::numberReg = std::regex(R"(^(\d+)(\.\d+)?)");
std::vector<char> Parser::ignoreChar = {' ', '\n', '\r', '\t'};
std::vector<char> Parser::endPiece = {'\n', '}', ']', ',', ':'};

JsonElement *json::parse(std::string &text) {
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
            continue;
        }

        // 根据字符判断后续解析行为
        charSwitch();

        if (result == nullptr) {
            result = currentElement;
        }
        currentPtr++;
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
    bool escape = false;
    // 字符指针留在结束引号后一位
    while (*currentPtr != '\0') {
        currentPtr++;
        location++;

        if (*currentPtr == '\\') {
            escape = !escape;
            if (escape) { innerString += *currentPtr; }
            continue;
        }
        if (!escape && *currentPtr == '"') {
            currentPtr++;
            return innerString;
        }
        innerString += *currentPtr;
    }
    throw JsonException("json: no close quote.");
}

char Parser::checkNextChar() const {
    const char *_temp = this->currentPtr;
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
    location++;
    switch (*currentPtr) {
        // 进入子map元素
        case '{':
            temp = new JsonElementMap();
            box.value = temp;
            currentElement->parseAdd(box);
            currentElement = temp;
            break;
            // 进入子list元素
        case '[':
            temp = new JsonElementSequence();
            box.value = temp;
            currentElement->parseAdd(box);
            currentElement = temp;
            break;
            // 退出map元素
        case '}':
            if (currentElement->typeCode() != 5) {
                throw JsonException("json: mismatched close char at " + std::to_string(location));
            }
            currentElement = container.empty() ? container.top() : nullptr;
            container.pop();
            break;
            // 退出list元素
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
            memoryString = innerQuote();
            if (typeid(currentElement) == typeid(JsonElementMap) && checkNextChar() == ':') {
                box.key = memoryString;
            } else {
                throw JsonException("json: except ':' after a key");
            }
            break;
        default:
            normalParse();
    }
}

void Parser::normalParse() {
    memoryString.clear();
    while (memoryString.length() < 50 && *currentPtr != '\0') {
        memoryString += *currentPtr;

        currentPtr++;
        location++;
    }
}
