
#include "JsonParser.h"
#include "sstream"
#include "stack"
#include "cstdint"

JsonElement *JsonParser::parse(string &text) {
    static char ignoreChar[] = {' ', '\n', '\r', '\t'};
    static char *pIgnoreEnd = ignoreChar + sizeof(ignoreChar) / ignoreChar[0];
    // store single element's content
    string content;
    // location of parser
    size_t location = 0;

    JsonElement *pJsonElement = nullptr;
    JsonElement *currentElement = pJsonElement;
    //  0 commons; 1 not string; 2 map element; 3 map; 4 sequence
    int8_t containerType = 0;

    stack<char> closeChar;

    while (location != text.size()) {
        char *pCurrentChar = &text.at(location);
        // ignore space
        if (std::find(ignoreChar, pIgnoreEnd, *pCurrentChar) == pIgnoreEnd) {
            continue;
        }
        location++;

        switch (*pCurrentChar) {
            case '"':
                content = JsonParser::innerQuote(text, location);
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
        string info = "json: mismatch ";
        while (!closeChar.empty()) { info += closeChar.top(); }
        throw JsonException(info);
    }
    return pJsonElement;
}

string JsonParser::innerQuote(string &text, size_t &location) {
    string result;
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


void JsonParser::createNumber(std::string &text, JsonElement *parentNode) {
    auto ele = new JsonElementNumber(text);
    ele->parentNode = parentNode;
    parentNode->childrenNode.push_back(ele);
}

void JsonParser::createString(std::string &text, JsonElement *parentNode) {
    auto ele = new JsonElementString(text);
    ele->parentNode = parentNode;
    parentNode->childrenNode.push_back(ele);
}

void JsonParser::createMapPare(std::string &text, JsonElement *parentNode) {

}

void JsonParser::createSequence(std::string &text, JsonElement *parentNode) {

}

void JsonParser::createMap(std::string &text, JsonElement *parentNode) {

}