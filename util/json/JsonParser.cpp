
#include "JsonParser.h"
#include "sstream"
#include "stack"
#include "cstdint"

using namespace json::element;

JsonElement *json::parse(std::string &text) {
    static std::vector<char> ignoreChar = {' ', '\n', '\r', '\t'};
    static std::regex number = std::regex(R"(^(\d+)(\.\d+)?)");
    char *pCurrentChar = &*text.begin();

    json::element::JsonPiece box;
    JsonElement *pJsonElement = nullptr;
    JsonElement *currentElement = nullptr;

    std::stack<JsonElement *> container;

    // main loop
    while (*pCurrentChar != '\n') {

        // ignore space
        pCurrentChar++;
        if (std::find(ignoreChar.begin(), ignoreChar.end(), *pCurrentChar) != ignoreChar.end()) {
            continue;
        }

        switch (*pCurrentChar) {

            case '{':
                currentElement = new JsonElementMap();
                break;
            case '[':
                currentElement = new JsonElementSequence();
                break;
            case '}':
                if (currentElement->typeCode() != 5) {
                    throw JsonException("json: mismatched close char at " + std::to_string(*pCurrentChar));
                }
                currentElement = container.empty() ? container.top() : nullptr;
                container.pop();
                break;
            case ']':
                if (currentElement->typeCode() != 6) {
                    throw JsonException("json: mismatched close char at "+ std::to_string(*pCurrentChar));
                }
                currentElement = container.empty() ? container.top() : nullptr;
                container.pop();
                break;
            default:
                box = json::decodePiece(pCurrentChar);
        }
        switch (box.valueType) {
            case 0:
                continue;
            case 1:
                currentElement = new JsonElementNull();
                currentElement->unifySetValue(box);
                break;
            case 2:

            case 3:
            case 4:
            default:
                ;
        }

        if (pJsonElement == nullptr) {
            pJsonElement = currentElement;
        }
    }
    if (!container.empty()) {
        throw JsonException("json: mismatched close char");
    }
    return pJsonElement;
}

// stop at end char
json::element::JsonPiece json::decodePiece(char *&pCurrentChar) {
    static std::vector<char> endChar = {'}', ']', ',', '\0'};
    static std::vector<char> ignoreChar = {' ', '\n', '\r', '\t'};
    static std::regex numberReg = std::regex(R"(^(-)?\d+(\.\d+)?$)");

    std::string content;
    auto box = JsonPiece();
    while (std::find(endChar.begin(), endChar.end(), *pCurrentChar) != endChar.end()) {
        // skip blank char
        if (std::find(ignoreChar.begin(), ignoreChar.end(), *pCurrentChar) == ignoreChar.end()) {
            pCurrentChar++;
            continue;
        }

        // decode key or string value
        if (*pCurrentChar == '"' && content.empty()) {
            std::string res = json::innerQuote(pCurrentChar);
            if (*pCurrentChar == ':') {
                box.key = res;
                box.parentType = 5;
            } else {
                box.value = res;
                box.valueType = 3;
                pCurrentChar++;
                return box;
            }
        }

        pCurrentChar++;
    }
    return box;
}

std::string json::innerQuote(char *&pCurrentChar) {
    std::string result;
    bool escape = false;
    // put ptr at first char which behind of end-quote
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




