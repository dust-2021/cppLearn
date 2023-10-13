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
    auto container_c =new json::JsonElementMap(*container);
    container->setValue("other", *container_c);

    std::cout << container->dump() << '\n';
    std::cout << container_c->dump() << '\n';
    delete container;
    delete container_c;
    return 0;
}