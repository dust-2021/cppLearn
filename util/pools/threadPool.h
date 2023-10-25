#ifndef FIRSTPROJECT_THREADPOOL_H
#define FIRSTPROJECT_THREADPOOL_H

# include "cstdint"
#include "vector"
#include "deque"
#include "thread"

namespace pools {
    class BaseThreadPool {
    public:
        uint8_t threads;
    private:
        std::vector<std::thread> worker;
    };
}


#endif //FIRSTPROJECT_THREADPOOL_H
