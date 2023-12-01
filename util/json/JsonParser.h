#ifndef FIRSTPROJECT_JSONPARSER_H
#define FIRSTPROJECT_JSONPARSER_H

#include "JsonElement.h"
#include "map"
#include "initializer_list"

// 外部访问空间
namespace json {

    element::JsonElement *parse(std::string &text);

}
namespace json::parser {
    // 解析异常对象
    class JsonException : public std::exception {
    public:
        std::string info;

        JsonException() = default;

        explicit JsonException(std::string &&info) : info(info) {};

        explicit JsonException(std::string &info) : info(info) {};

        [[nodiscard]] const char *what() const noexcept override { return this->info.c_str(); };
    };

    // 解析对象
    class Parser {
    public:
        // 无视字符
        static std::vector<char> ignoreChar;
        // 数字正则规则
        static std::regex numberReg;
        // 单元结束符
        static std::vector<char> endPiece;
        // 容器结束单元
        static std::unordered_map<int, std::vector<char>> endContainer;

        explicit Parser(std::string &text) : _text(text) { currentPtr = text.c_str(); };

        explicit Parser(std::string &&text) : _text(text) { currentPtr = text.c_str(); };

        json::element::JsonElement *parse();


    private:
        // 待解析字符串
        std::string _text;

        // 解析结果
        json::element::JsonElement *result = nullptr;

        // 当前json元素
        json::element::JsonElement *currentElement = nullptr;

        // 临时切换对象
        json::element::JsonElement *temp = nullptr;

        // 当前字符
        const char *currentPtr = nullptr;

        // 当前字符位置
        size_t location = 0;

        // 缓存字符串
        std::string memoryString;

        // 初始化json元素的结构体
        json::element::JsonPiece box;

        // 嵌套容器存储
        std::stack<json::element::JsonElement *> container;

        // 解析双引号内容
        std::string innerQuote();

        // 读取非引号内字符
        void normalParse();

        // 逐字符解析
        void charSwitch();

        // 每当一个字符被解析 字符指针和位置记录加一
        void advance(int num = 1);

        // 从当前字符开始 检查下一个有效字符
        [[nodiscard]] char checkNextChar(size_t &&offset = 0, bool backStep = false) const;
    };
}
#endif //FIRSTPROJECT_JSONPARSER_H
