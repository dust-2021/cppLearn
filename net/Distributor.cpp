
#include "Distributor.h"



// initialize single object
const Distributor *Distributor::distributor = nullptr;

void Distributor::startListen(int threadCount) {
    for (int i = 0; i < threadCount; ++i) {
        auto t = new thread();
        this->theadId.push_back(t->get_id());
        t->detach();
    }

}

const Distributor *Distributor::getUniqueInstance() {
    if(distributor == nullptr){
        distributor = new Distributor();
    }
    return distributor;
}
