#include <iostream>
#include <utility>
#include "fstream"
#include "util/json/elements.h"
#include "chrono"
#include "unordered_map"

void element() {
    json::json js;
    js = {{"Cookie", "123456"}, {"Content-Type", "application/json"}, {"name", "大"}};
    js["token"] = "1235";
//    js["location"] = json::Null;
    auto res = js.dump();

    std::wcout.imbue(std::locale("zh_CN.gb2312"));
    std::wcout << res << '\n';
}

void test() {
    int **a;
    int *b = new int(1);
    a = &b;
    std::cout << a << '\n' << *a << '\n' << **a << '\n';
    delete b;
};

int main() {
    element();
}



