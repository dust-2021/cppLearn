#ifndef FIRSTPROJECT_JSONPARSER_H
#define FIRSTPROJECT_JSONPARSER_H

#include "JsonElement.h"
#include "map"

// 外部访问空间
namespace json {

    element::JsonElement *parse(std::string &text);

    // cpp对象转json对象
    template<class T_con, class T_ele>
    class JsonObject {
        element::JsonElement *value = nullptr;

        T_ele operator[](std::string &&key) {
            if (value == nullptr) {
                value = new element::JsonElementMap();
            }
        }

        T_ele operator[](size_t &&key) {

        }
    };
}
namespace json::parser {

    class JsonException : public std::exception {
    public:
        std::string info;

        JsonException() = default;

        explicit JsonException(std::string &&info) : info(info) {};

        explicit JsonException(std::string &info) : info(info) {};

        [[nodiscard]] const char *what() const noexcept override { return this->info.c_str(); };
    };

    class Parser {
    public:
        // 无视字符
        static std::vector<char> ignoreChar;
        // 数字正则规则
        static std::regex numberReg;
        // 单元结束符
        static std::vector<char> endPiece;
        // 容器结束单元
        static std:: unordered_map<int, std::vector<char>> endContainer;

        explicit Parser(std::string &text) : _text(text) { currentPtr = text.c_str(); };

        json::element::JsonElement *parse();


    private:
        // 待解析字符串   `
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
        // 跳过字符标识
        bool afterIgnore = false;
        // 是否转义
        bool escape = false;
        // 指定字符
        std::vector<char> designChar;
        // 初始化json元素的
        json::element::JsonPiece box;

        // 嵌套容器存储
        std::stack<json::element::JsonElement *> container;

        // 解析双引号内容
        std::string innerQuote();

        // 读取非引号内字符
        void normalParse();

        void charSwitch();

        // 检查下一个有效字符
        [[nodiscard]] char checkNextChar(size_t &&offset=0) const;
    };
}
#endif //FIRSTPROJECT_JSONPARSER_H
