
#ifndef FIRSTPROJECT_DISTRIBUTOR_H
#define FIRSTPROJECT_DISTRIBUTOR_H

#include "../util/http/Request.h"
#include "../util/http/Response.h"
#include "thread"
#include "vector"

class Distributor {
    // global unique instance class.
public:
    static const Distributor *distributor;

    ~Distributor() = delete;

    Distributor(Distributor &distributor) = delete;

    Distributor &operator=(const Distributor &distributor) = delete;

    static const Distributor * getUniqueInstance();

    void httpHandler(Request &request);

    void tcpHandler();

    void udpHandler();

private:
    // recorde threads' id
    vector<thread::id> theadId;
    // recorde how many task every single thread done
    unordered_map<thread::id, int> executeCountMap;

    Distributor() = default;

    void startListen(int threadCount);

};

#endif //FIRSTPROJECT_DISTRIBUTOR_H
