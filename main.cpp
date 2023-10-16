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
    container->setValue("name", *ele);
    auto container_g =container->getCopy();
    container->setValue("g", *container_g);

    std::cout << container->dump() << '\n';
    std::cout << container_g->dump() << '\n';
    delete container;
    delete container_g;
    return 0;
}