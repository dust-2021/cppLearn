#ifndef FIRSTPROJECT_JSONPARSER_H
#define FIRSTPROJECT_JSONPARSER_H

#include "JsonElement.h"
#include "map"

namespace json {

    element::JsonElement *parse(std::string &text);

    // convert obj to json obj
    template<class T>
    element::JsonElement *load(T any);


    template<class T>
    element::JsonElement *load(std::vector<T> any);

    template<class T>
    element::JsonElement *load(std::map<std::string, T> any);

    static void decodePiece(element::JsonElement *&current);

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
