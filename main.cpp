#include <iostream>
#include "fstream"
#include "util/json/JsonElement.h"

using namespace std;


int main() {
    auto b = JsonElementBool(true);
    auto str = b.dump();
    cout << str << endl;
    delete str;
}