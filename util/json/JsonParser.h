#ifndef FIRSTPROJECT_JSONPARSER_H
#define FIRSTPROJECT_JSONPARSER_H

#include "JsonElement.h"
#include "map"

namespace json {

    element::JsonElement *parse(std::string &text);

    template<class T>
    element::JsonElement *load(T &any) {
        return new element::JsonElementNumber(std::to_string(any));
    }

    // convert obj to json obj
//    template<> element::JsonElement *load<std::string>(std::string &any) {
//        return new element::JsonElementString(any);
//    }

//    element::JsonElement *load() {
//        return new element::JsonElementNull();
//    }


    template<class T>
    element::JsonElement *load(std::vector<T> any) {
        auto obj = new element::JsonElementSequence();
        for (auto p: any) {
            obj->push_back(load(p));
        }
        return obj;
    }

    template<class T>
    element::JsonElement *load(std::map<std::string, T> any) {
        auto obj = new element::JsonElementMap();
        for (auto pair: any) {
            obj[pair.first] = load(pair.second);
        }
        return obj;
    }

    template<class T>
    element::JsonElement *load(std::unordered_map<std::string, T> any) {
        auto obj = new element::JsonElementMap();
        for (auto pair: any) {
            obj[pair.first] = load(pair.second);
        }
        return obj;
    }

    static json::element::JsonPiece decodePiece(char *&pCurrentChar);

    static std::string innerQuote(char *&pCurrentChar);


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
