#include <iostream>
#include <utility>
#include "fstream"
#include "util/json/JsonElement.h"
#include "util/json/JsonParser.h"
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

    std::cout << j->dump() << '\n';

    delete j;
}

int main() {
    std::unordered_map<std::string, json::element::JsonElement> _map;
    _map["age"];
    for (const auto &pair: _map) {
        std::cout << pair.first << ':' << pair.second.dump() << '\n';
    }
}



