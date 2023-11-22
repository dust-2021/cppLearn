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
        static std::vector<char> endPiece;

        explicit Parser(std::string &text) : _text(text) {currentPtr = text.c_str();};

        json::element::JsonElement *parse();


    private:
        // 待解析字符串   `
        std::string _text;

        // 解析结果
        json::element::JsonElement *result = nullptr;
        // 当前json元素
        json::element::JsonElement *currentElement = nullptr;

        // 临时切换对象
        json::element::JsonElement* temp = nullptr;

        // 当前字符
        const char *currentPtr = nullptr;
        // 当前字符位置
        size_t location = 0;
        // 缓存字符串
        std::string memoryString;
        // 跳过字符标识
        bool afterIgnore = false;
        // 指定字符
        std::vector<char> designChar;
        // 初始化json元素的
        json::element::JsonPiece box;

        // 嵌套容器存储
        std::stack<json::element::JsonElement *> container;

        // 解析双引号内容
        static std::string innerQuote(char *&pCurrentChar);

        // 读取非引号内字符
        void normalParse();

        void charSwitch();

        // 检查下一个有效字符
        [[nodiscard]] char checkNextChar() const;
    };
}
#endif //FIRSTPROJECT_JSONPARSER_H
