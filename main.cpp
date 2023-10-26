#include <iostream>
#include "fstream"
#include "util/json/JsonElement.h"
#include "util/json/JsonParser.h"
#include "cstdio"
#include "auto_ptr.h"
#include "functional"
#include "unordered_map"

json::element::JsonElement *func() {
    std::ifstream f("/code/cppCode/firstProject/test/data.json");
    std::string res;
    if (f) {
        printf("opening\n");
        std::string line;
        while (std::getline(f, line)) {
            res += line;
        }
    } else {
        std::cout << "open failed\n";
    }
    std::cout << res << '\n';
    auto result = json::parse(res);
    return result;
}

void func2() {
    auto data = std::unordered_map<std::string, int>{{"age", 12},
                                                     {"loc", 13}};
    data["name"];
    data["name"] = 11;
    std::cout << data["name"] << '\n';
}

void func3() {
    std::string str = "12345";
    const char *p = str.c_str();
    while (*p != '\0') {
        std::cout << *p << '\n';
        p++;
    }
}

int main() {
    auto data = func();
    std::cout << data->dump() << '\n';
}