#include "elements.h"

using namespace json::elements;
jsonIter valueType::begin() {
    throw jsonError("element not support traverse");
}

jsonIter valueType::end() {
    throw jsonError("element not support traverse");
}
