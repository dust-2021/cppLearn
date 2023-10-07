
#ifndef FIRSTPROJECT_PYTHONEXECUTOR_H
#define FIRSTPROJECT_PYTHONEXECUTOR_H

#include "string"

using namespace std;

class PythonExecutor {
public:
    static string py_Version;
    static bool checkPy();

};

bool PythonExecutor::checkPy() {
    return false;
}

#endif //FIRSTPROJECT_PYTHONEXECUTOR_H
