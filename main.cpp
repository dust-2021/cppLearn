#include <iostream>
#include "fstream"
#include "util/json/JsonElement.h"
#include "cstdio"
#include "auto_ptr.h"
#include "functional"
#include "unordered_map"


int main() {
    std::unordered_map<std::string, int> dict = {{"name", 1}};
    auto container = new json::JsonElementMap();
    auto ele = new json::JsonElementNull();
    container->childrenNode["name"] = ele->getCopy();
    auto container_g =container->getCopy();
    container->childrenNode["g"] = container_g->getCopy();

    std::cout << container->dump() << '\n';
    std::cout << container_g->dump() << '\n';
    delete container;
    delete container_g;
    return 0;
}