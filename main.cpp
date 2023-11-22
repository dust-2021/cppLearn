#include <iostream>
#include "fstream"
#include "util/json/JsonElement.h"
#include "util/json/JsonParser.h"
#include "cstdio"
#include "auto_ptr.h"
#include "functional"
#include "unordered_map"

void func2() {
    auto data = std::unordered_map<std::string, int>{
        {"age", 12},
        {"loc", 13}
    };
    data["name"];
    data["name"] = 11;
    std::cout << data["name"] << '\n';
}

void func3() {
    std::string str = "12345";
    const char* p = str.c_str();
    while (*p != '\0') {
        std::cout << *p << '\n';
        p++;
    }
}

void func3(int* a) {
    std::cout << &a << '\n';
}

int main() {
    std::string text = R"({"name": "Tom", "age": 12.2, "product": [""]})";
    auto json_obj = json::parse(text);
    std::cout << json_obj->dump() << '\n';
}
