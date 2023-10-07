#ifndef FIRSTPROJECT_JSONOBJECT_H
#define FIRSTPROJECT_JSONOBJECT_H
#include "jsonElement.h"
#include "string"

class JsonObject {

public:

    static int maxJsonDepth;
    int selfMaxJsonDepth;
    // always on the heap
    JsonElement *rootNode;

    string originString;

    explicit JsonObject(string &text, int maxDepth = 0);

private:

};


#endif //FIRSTPROJECT_JSONOBJECT_H
