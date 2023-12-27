#include <iostream>
#include <utility>
#include "fstream"
#include "util/json/elements.h"
#include "chrono"
#include "unordered_map"

void dump_to(const char *path, std::string &text) {
    std::ofstream out(path);
    if (out.is_open()) {
        out << text;
        out.close();
    }
}

void element() {
    json::json js;
    js = {{"Cookie",       "123456"},
          {"Content-Type", "application/json"},
          {"name",         "大大大"}};
    js["name"] = 13;
    js["age"] = 12;
//    js["Cookie"] = json::Null;

//    std::cout << json::Null.dump() << '\n';
    auto res = js.dump();
    auto stores = res.substr(1, res.size() - 2);
    dump_to(R"(C:\code\cppCode\firstProject\test\data.json)", stores);

    system("chcp 65001");
    std::cout << res << '\n';
}

void test_null() {
    auto a = json::json();
    auto b = json::json();
    a = json::Null;
    b = json::Null;
    std::cout << a.dump() << b.dump() << '\n';
}

void test(){

}



int main() {
    element();
}



