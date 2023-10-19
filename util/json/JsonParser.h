

#ifndef FIRSTPROJECT_JSONPARSER_H
#define FIRSTPROJECT_JSONPARSER_H

#include "JsonElement.h"

namespace json {

    JsonElement *parse(std::string &text);

    static void decodePiece(JsonElement *&current);

    static std::string innerQuote(std::string &text, std::size_t &location);


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
}
#endif //FIRSTPROJECT_JSONPARSER_H
