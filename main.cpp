#include <iostream>
#include <utility>
#include "fstream"
#include "util/json/JsonElement.h"
#include "util/json/JsonParser.h"


int main() {
    std::fstream f;
    f.open(R"(/code/cppCode/firstProject/test/data.json)", std::ios::in);
    std::string res;
    if (f.is_open()) {
        std::string line;
        while (std::getline(f, line)) {
            res += line;
            }
        }

    f.close();
    
    std::cout << res << '\n';

    auto j = json::parse(res);

    std::cout << j->dump() << '\n';
}
