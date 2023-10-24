
#include "JsonParser.h"
#include "sstream"
#include "stack"
#include "cstdint"

using namespace json::element;

JsonElement *json::parse(std::string &text) {
    static std::vector<char> ignoreChar = {' ', '\n', '\r', '\t'};
    static std::regex number = std::regex(R"(^(\d+)(\.\d+)?)");
    // store single element's content
    std::string content;
    // location of parser
    size_t location = 0;
    const char *pCurrentChar = text.c_str();
    bool nameType = false;

    JsonPiece box;
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
            case '"':
                if (nameType) {
                    box.key = innerQuote(text, location);
                    nameType = false;
                } else {
                    currentElement = new JsonElementString();
                    box.value = innerQuote(text, location);
                    box.active = true;
                }
                break;
            case '{':
                currentElement = new JsonElementMap();
                nameType = true;
                break;
            case '[':
                currentElement = new JsonElementSequence();
                break;
            case ',':
                box.active = true;
                break;
            case ':':
                nameType = false;
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
            default:
                content += *pCurrentChar;
        }

        // box active means to add value to current element
        if (!box.active) {
            continue;
        } else if (content == "null") {
            currentElement = new JsonElementNull();
            box.active = true;
        } else if (content == "true" || content == "false") {
            currentElement = new JsonElementBool();
            box.flag = content == "true";
            box.active = true;
        } else if (std::regex_match(content, number)) {
            currentElement = new JsonElementNumber();
            box.value = content;
            box.active = true;
        } else {
            throw JsonException("json: bad_convert: " + content);
        }

        currentElement->unifySetValue(box);
        box.active = false;

        if (pJsonElement == nullptr) {
            pJsonElement = currentElement;
        }
    }
    if (!container.empty()) {
        throw JsonException("json: mismatched close char");
    }
    return pJsonElement;
}

json::element::JsonPiece decodePiece(char *&pCurrentChar) {
    static std::vector<char> endChar = {'}', ']', ',', '\0'};
    static std::vector<char> ignoreChar = {' ', '\n', '\r', '\t'};
    static std::regex numberReg = std::regex(R"(^(-)?\d+(\.\d+)?$)");

    auto box = JsonPiece();
    while (std::find(endChar.begin(), endChar.end(), *pCurrentChar) != endChar.end()) {

        pCurrentChar++;
    }
    return box;
}

std::string json::innerQuote(std::string &text, size_t &location) {
    std::string result;
    bool escape;
    while (location != text.size()) {
        char *pCurrentChar = &text.at(location);
        location++;

        if (*pCurrentChar == '\\') {
            escape = !escape;
            if (escape) { result += *pCurrentChar; }
            continue;
        }
        if (!escape && *pCurrentChar == '"') {
            break;
        }
        result += *pCurrentChar;
    }
    return result;
}




