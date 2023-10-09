
#include "JsonParser.h"
#include "sstream"
#include "stack"
#include "cstdint"

JsonElement* parse(std::string &text) {
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

    std::stack<char> closeChar;

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
                if (pJsonElement == nullptr) {
                    pJsonElement = new JsonElementString(content);
                }
                continue;
            case '{':
                closeChar.push('}');


                continue;
            case '[':
                closeChar.push(']');
                continue;
            case ',':
                if (containerType){

                }
            case ':':

            default:
                content += pCurrentChar;
        }
        throw JsonException("json: bad");
    }
    if (!closeChar.empty()) {
        std::string info = "json: mismatch ";
        while (!closeChar.empty()) { info += closeChar.top(); }
        throw JsonException(info);
    }
    return pJsonElement;
}

std::string innerQuote(std::string &text, size_t &location) {
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


void createNumber(std::string &text, JsonElement *parentNode) {

}

void createString(std::string &text, JsonElement *parentNode) {

}

void createMapPare(std::string &text, JsonElement *parentNode) {

}

void createSequence(std::string &text, JsonElement *parentNode) {

}

void createMap(std::string &text, JsonElement *parentNode) {

}