#include <iostream>
#include "fstream"
#include "util/json/JsonElement.h"
#include "cstdio"
#include "auto_ptr.h"

class A {
public:
    std::vector<int> data;

    void add(int &&v){ this->data.push_back(v);}
};

int main() {
    auto a = A();
    a.add(1);
    auto b  = A(a);
    std::cout << a.data.size() << "--" << b.data.size() << std::endl;
}