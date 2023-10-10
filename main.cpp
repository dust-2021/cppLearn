#include <iostream>
#include "fstream"
#include "util/json/JsonElement.h"
#include "cstdio"
#include "auto_ptr.h"

int main() {
    std::unique_ptr<JsonElementString> a(new JsonElementString("mf"));
    auto b = JsonElementMap();
    b.setValue("name", a);
    auto c = JsonElementSequence();
    c.addValue(a);
    std::cout <<a->dump() << std::endl;

    std::cout<< typeid(a).name() << std::endl;
    std::cout<< typeid(b.getElement("name")).name() << std::endl;
    std::cout << b.dump() << std::endl;
    std::cout << c.dump() << std::endl;
}