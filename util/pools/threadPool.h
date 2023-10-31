#ifndef FIRSTPROJECT_THREADPOOL_H
#define FIRSTPROJECT_THREADPOOL_H

#include "cstdint"
#include "vector"
#include "deque"
#include "thread"
#include "functional"

namespace pools {
    class BaseThreadPool {
    public:
        uint8_t threads;

        virtual void start();

        virtual void close();

        virtual void submit();

    private:
        std::vector<std::thread> worker;

    };
}


#endif //FIRSTPROJECT_THREADPOOL_H
