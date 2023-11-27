#include <iostream>
#include <utility>
#include "fstream"
#include "util/json/JsonElement.h"
#include "util/json/JsonParser.h"
#include "filesystem"

int main() {
    std::fstream f;
    f.open(R"(C:\code\cppCode\firstProject\test\data.json)", std::ios::in);
    std:: cout<< std::filesystem::current_path() << '\n';
    std::string res;
    if (f.is_open()) {
        std::string line;
        while (std::getline(f, line)) {
            for (char p: line) {
                if (p == '\\' || p == '"'){
                    res += '\\';
                }
                res +=p;
            }
        }
    }
    f.close();
    
    std::cout << res << '\n';

    auto j = json::parse(res);

    std::cout << j->dump() << '\n';
}
