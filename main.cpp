#include <iostream>
#include "fstream"
#include "util/json/JsonElement.h"
#include "cstdio"
#include "auto_ptr.h"
#include "functional"
#include "unordered_map"

int main() {
    std::unordered_map<std::string, int> dict = {{"name", 1}};
    auto container = new JsonElementMap();
    auto ele = new JsonElementNull();
    container->setValue("name", ele);
    std::cout << container->getValue("name").typeCode << '\n';
    auto container_c = elementCopy(container);
    std::cout << container->dump() << '\n';
    std::cout << container_c->dump() << '\n';

}