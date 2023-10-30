
#include "JsonParser.h"
#include "sstream"
#include "stack"
#include "cstdint"

using namespace json::element;

JsonElement *json::parse(std::string &text) {
    static std::vector<char> ignoreChar = {' ', '\n', '\r', '\t'};
    static std::regex number = std::regex(R"(^(\d+)(\.\d+)?)");
    char *pCurrentChar = &*text.begin();
    size_t location = -1;

    json::element::JsonPiece box;
    JsonElement *pJsonElement = nullptr;
    JsonElement *currentContainer = nullptr;

    std::string mem;
    bool endPiece = false;

    std::stack<JsonElement *> container;

    // main loop
    while (*pCurrentChar != '\0') {
        location++;

        // ignore space
        if (std::find(ignoreChar.begin(), ignoreChar.end(), *pCurrentChar) != ignoreChar.end()) {
            pCurrentChar++;
            continue;
        }

        if (endPiece){

        }

        switch (*pCurrentChar) {

            case '{':
                currentContainer = new JsonElementMap();
                break;
            case '[':
                currentContainer = new JsonElementSequence();
                break;
            case '}':
                if (currentContainer->typeCode() != 5) {
                    throw JsonException("json: mismatched close char at " + std::to_string(location));
                }
                currentContainer = container.empty() ? container.top() : nullptr;
                container.pop();
                break;
            case ']':
                if (currentContainer->typeCode() != 6) {
                    throw JsonException("json: mismatched close char at " + std::to_string(location));
                }
                currentContainer = container.empty() ? container.top() : nullptr;
                container.pop();
                break;
            case '"':
                if (!mem.empty()) {
                    throw JsonException("json: unexpect \" at " + std::to_string(location));
                }
                mem = innerQuote(pCurrentChar);
                endPiece = true;
                break;
            default:
                // load an element, clear box
                box = json::decodePiece(pCurrentChar);
                currentContainer->parseAdd(box);
                box.value = nullptr;
                box.key.clear();
        }

        if (pJsonElement == nullptr) {
            pJsonElement = currentContainer;
        }
        pCurrentChar++;
    }
    if (!container.empty()) {
        throw JsonException("json: mismatched close char");
    }
    return pJsonElement;
}

// stop at end char
json::element::JsonPiece json::decodePiece(char *&pCurrentChar) {
    static std::vector<char> endChar = {'[', '{', '}', ']', ',', '\0'};
    static std::vector<char> ignoreChar = {' ', '\n', '\r', '\t'};
    static std::regex numberReg = std::regex(R"(^(-)?\d+(\.\d+)?$)");

    std::string content;
    auto box = JsonPiece();

    while (std::find(endChar.begin(), endChar.end(), *pCurrentChar) == endChar.end()) {
        // skip blank char
        if (std::find(ignoreChar.begin(), ignoreChar.end(), *pCurrentChar) != ignoreChar.end()) {
            pCurrentChar++;
            continue;
        }

        // decode key or string value
        if (*pCurrentChar == '"' && content.empty()) {

            std::string res = json::innerQuote(pCurrentChar);
            if (*pCurrentChar == ':') {
                box.key = res;
                pCurrentChar++;
                continue;
            } else {
                box.value = new JsonElementString(res);
                return box;
            }

        } else if (*pCurrentChar == '"') {
            throw JsonException("json: unexpect '\"'");
        }

        content += *pCurrentChar;

        pCurrentChar++;
    }

    if (content == "null") {
        box.value = new JsonElementNull();
    } else if (std::regex_match(content, numberReg)) {
        box.value = new JsonElementNumber(content);
    } else if (content == "true" or content == "false") {
        box.value = new JsonElementBool(content == "true");
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




