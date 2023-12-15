#include <iostream>
#include <utility>
#include "fstream"
#include "util/json/JsonElement.h"
#include "util/json/JsonParser.h"
#include "util/json/elements.h"
#include "chrono"
#include "unordered_map"

void func() {
    std::fstream f;
    f.open(R"(../test/data.json)", std::ios::in);
    std::string res;
    if (f.is_open()) {
        std::string line;
        while (std::getline(f, line)) {
            res += line;
        }
    }

    f.close();

    std::cout << res << '\n';
    auto now = std::chrono::high_resolution_clock::now();
    auto j = json::parse(res);
    auto after = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = after - now;
    std::cout << elapsed_seconds.count() << '\n';

    std::cout << (*j)["products"]->dump() << '\n';
    delete j;
}

void element(){
    json::json js;
    js["name"];
    js["name"] = 1;
    auto temp = js["name"];
    temp = 12;
}

void test() {
    int ** a;
    int *b = new int(1);
    a = &b;
    std::cout << a << '\n' << *a << '\n' << **a << '\n';
    delete b;
};

int main() {
    element();
}



