#include <iostream>
#include "fstream"
#include "util/json/JsonElement.h"
#include "cstdio"
#include "auto_ptr.h"

void func(int *n) {
    std::cout << *n << '\n' << "地址:" <<&n << '\n';
}


int main() {
    int *a = new int(1);
    std::cout << &a << '\n';
    func(a);
}