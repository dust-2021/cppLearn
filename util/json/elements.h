#ifndef FIRSTPROJECT_ELEMENTS_H
#define FIRSTPROJECT_ELEMENTS_H

#include "string"
#include "iostream"

namespace json::elements {
    class valueType;

    class nullType;

    class boolType;

    class numberType;

    class stringType;

    class mapType;

    class arryType;

    template<class Args>
    class json;

    class valueType {
    public:

        valueType() = default;

    private:
    };

    template<class Args>
    class json {

    public:

        json() = default;

        json(std::initializer_list<Args> list) {

        }

        json(std::initializer_list<std::initializer_list<Args>> pairs) {

        }

    private:
        valueType *_value = new valueType();
    };

}


#endif //FIRSTPROJECT_ELEMENTS_H
