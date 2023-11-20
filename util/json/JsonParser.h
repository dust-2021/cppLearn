#ifndef FIRSTPROJECT_JSONPARSER_H
#define FIRSTPROJECT_JSONPARSER_H

#include "JsonElement.h"
#include "map"

namespace json {

    element::JsonElement *parse(std::string &text);

    class JsonException : public std::exception {
    public:
        std::string info;

        JsonException() = default;

        explicit JsonException(std::string &&info) : info(info) {};

        explicit JsonException(std::string &info) : info(info) {};

        [[nodiscard]] const char *what() const noexcept override { return this->info.c_str(); };
    };
}
namespace json::parser {
    class Parser {
    public:
        static std::vector<char> ignoreChar;
        static std::regex numberReg;

        explicit Parser(std::string &text) : _text(text) {};

        json::element::JsonElement *parse();

        static std::string innerQuote(char *&pChar);

        char checkNextChar();

        void charSwitch();

    private:
        // 待解析字符串   `
        std::string _text;

        // 解析结果
        json::element::JsonElement *result = nullptr;
        // 当前json元素
        json::element::JsonElement *currentElement = nullptr;

        // 当前字符
        char *currentPtr = nullptr;
        // 当前字符位置
        size_t location = 0;
        // 缓存字符串
        std::string memoryString;
        // 跳过字符标识
        bool afterIgnore = false;

        // 指定标识
        bool design = false;
        // 指定字符
        std::vector<char> designChar;
        // 初始化json元素的
        json::element::JsonPiece box;

        // 嵌套容器存储
        std::stack<json::element::JsonElement *> container;
    };
}
#endif //FIRSTPROJECT_JSONPARSER_H
