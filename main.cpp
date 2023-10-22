#include <iostream>
#include "fstream"
#include "util/json/JsonElement.h"
#include "util/json/JsonParser.h"
#include "cstdio"
#include "auto_ptr.h"
#include "functional"
#include "unordered_map"


int main() {
//    std::string text = "{\"name\": null}";
//    auto res = json::parse(text);
//    std::cout << res->dump() << '\n';
    std::fstream f("/code/cppCode/firstProject/test/data.json");
    std::string res;
    if (f){
        printf("opening");
        std::string line;
        while (std::getline(f,line )){
            res += line;
        }
    }
    std::cout<< res << '\n';
    auto result = json::parse(res);

}