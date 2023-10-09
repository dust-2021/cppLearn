

#ifndef FIRSTPROJECT_JSONPARSER_H
#define FIRSTPROJECT_JSONPARSER_H

#include "JsonElement.h"


JsonElement *parse(std::string &text);

std::string *dump(JsonElement &obj);

static JsonElement* decodePiece();

static std::string innerQuote(std::string &text, std::size_t &location);

static void createNumber(std::string &text, JsonElement *parentNode);

static void createString(std::string &text, JsonElement *parentNode);

static void createMapPare(std::string &text, JsonElement *parentNode);

static void createMap(std::string &text, JsonElement *parentNode);

static void createSequence(std::string &text, JsonElement *parentNode);

class JsonException : public std::exception {
public:
    std::string info;

    JsonException() = default;

    explicit JsonException(std::string &&info) : info(info) {};

    explicit JsonException(std::string &info) : info(info) {};

    [[nodiscard]] const char *what() const noexcept override {
        return this->info.c_str();
    };
};

#endif //FIRSTPROJECT_JSONPARSER_H
