#include <iostream>
#include "fstream"
#include "util/json/JsonElement.h"
#include "util/json/JsonParser.h"
#include "cstdio"
#include "auto_ptr.h"
#include "functional"
#include "unordered_map"

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
    int * a ;
    int b = 12;
    a = &b;
    std::cout << a << '\n';

}