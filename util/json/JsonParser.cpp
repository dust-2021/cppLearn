
#include "JsonParser.h"
#include "sstream"
#include "stack"
#include "cstdint"

using namespace json;

JsonElement* json::parse(std::string &text) {
    static char ignoreChar[] = {' ', '\n', '\r', '\t'};
    static char *pIgnoreEnd = ignoreChar + sizeof(ignoreChar) / ignoreChar[0];
    // store single element's content
    std::string content;
    // location of parser
    size_t location = 0;

    JsonElement *pJsonElement = nullptr;
    JsonElement *currentElement = pJsonElement;

    //  0 commons; 1 not string; 2 map element; 3 map; 4 sequence
    std::int8_t containerType = 0;

    std::stack<JsonElement> container;

    while (location != text.size()) {
        char *pCurrentChar = &text.at(location);
        // ignore space
        if (std::find(ignoreChar, pIgnoreEnd, *pCurrentChar) == pIgnoreEnd) {
            continue;
        }
        location++;

        switch (*pCurrentChar) {
            case '"':
                content = innerQuote(text, location);
                currentElement = new JsonElementString();
                continue;
            case '{':



                continue;
            case '[':

                continue;
            case ',':
                if (containerType){

                }
            case ':':

            default:
                throw JsonException("json: bad_char:" + std::to_string(location) +" '" + *pCurrentChar + '\'');
        }
        if (pJsonElement == nullptr){
            pJsonElement = currentElement;
        }
    }
    if (!container.empty()) {
        throw JsonException("json: mismatch");
    }
    return pJsonElement;
}

std::string json::innerQuote(std::string &text, size_t &location) {
    std::string result;
    bool escape;
    while (location != text.size()) {
        char *pCurrentChar = &text.at(location);
        location++;

        if (*pCurrentChar == '\\'){
            escape = !escape;
            if (escape){result += *pCurrentChar;}
            continue;
        }
        if(!escape && *pCurrentChar == '"'){
            break;
        }
        result += *pCurrentChar;
    }
    return result;
}


