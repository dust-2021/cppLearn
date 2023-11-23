#include <iostream>
#include <utility>
#include "fstream"
#include "util/json/JsonElement.h"
#include "util/json/JsonParser.h"
#include "cstdio"
#include "auto_ptr.h"
#include "functional"
#include "unordered_map"

int main() {
    std::fstream f;
    f.open(R"(C:\code\cppCode\firstProject\test\data.json)", std::ios::in);
    std::string res;
    if (f.is_open()) {
        std::string line;
        while (std::getline(f, line)) {
            res += line;
        }
    }

    auto j = json::parse(res);

    std::cout << j->dump() << '\n';
}
