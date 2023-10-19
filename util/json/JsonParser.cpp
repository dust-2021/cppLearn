
#include "JsonParser.h"
#include "sstream"
#include "stack"
#include "cstdint"

using namespace json;

JsonElement *json::parse(std::string &text) {
    static char ignoreChar[] = {' ', '\n', '\r', '\t'};
    static char *pIgnoreEnd = ignoreChar + sizeof(ignoreChar) / ignoreChar[0];
    static std::regex number = std::regex(R"(^(\d+)(\.\d+)?)");
    // store single element's content
    std::string content;
    // location of parser
    size_t location = 0;
    bool nameType = false;

    JsonPiece box;
    JsonElement *pJsonElement = nullptr;
    JsonElement *currentElement = nullptr;

    std::stack<JsonElement *> container;

    // main loop
    while (location != text.size()) {
        char *pCurrentChar = &text.at(location);
        // ignore space
        location++;
        if (std::find(ignoreChar, pIgnoreEnd, *pCurrentChar) != pIgnoreEnd) {
            continue;
        }

        switch (*pCurrentChar) {
            case '"':
                if (nameType){
                    box.key = innerQuote(text, location);
                    nameType = false;
                } else{
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
                box.active= true;
                break;
            case ':':
                nameType = false;
                break;
            case '}':
                if(currentElement->typeCode() != 5){
                    throw JsonException("json: mismatched close char at " + std::to_string(location));
                }
                container.pop();
                currentElement = container.top();
                break;
            case ']':
                if (currentElement->typeCode() != 6){
                    throw JsonException("json: mismatched close char at " + std::to_string(location));
                }
                container.pop();
                currentElement = container.top();
                break;
            default:
                content += *pCurrentChar;
        }
        if (!box.active){
            continue;
        } else if (content == "null"){
            currentElement = new JsonElementNull();
            box.active= true;
        } else if(content == "true" || content == "false"){
            currentElement = new JsonElementBool();
            box.flag = content == "true";
            box.active= true;
        }
        else if (std::regex_match(content, number)){
            currentElement = new JsonElementNumber();
            box.value = content;
            box.active= true;
        } else{
            throw JsonException("json: bad_convert: " + content);
        }

        currentElement->unifySetValue(box);
        box.active= false;

        if (pJsonElement == nullptr) {
            pJsonElement = currentElement;
        }
    }
    if (!container.empty()) {
        throw JsonException("json: mismatched close char");
    }
    return pJsonElement;
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


