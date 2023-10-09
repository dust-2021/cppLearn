

#ifndef FIRSTPROJECT_JSONPARSER_H
#define FIRSTPROJECT_JSONPARSER_H

#include "JsonElement.h"

using namespace std;


JsonElement *parse(string &text);

string *dump(JsonElement &obj);

static JsonElement* decodePiece();

static string innerQuote(string &text, size_t &location);

static void createNumber(string &text, JsonElement *parentNode);

static void createString(string &text, JsonElement *parentNode);

static void createMapPare(string &text, JsonElement *parentNode);

static void createMap(string &text, JsonElement *parentNode);

static void createSequence(string &text, JsonElement *parentNode);

class JsonException : public exception {
public:
    string info;

    JsonException() = default;

    explicit JsonException(string &&info) : info(info) {};

    explicit JsonException(string &info) : info(info) {};

    [[nodiscard]] const char *what() const noexcept override {
        return this->info.c_str();
    };
};

#endif //FIRSTPROJECT_JSONPARSER_H
