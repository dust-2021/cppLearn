#ifndef FIRSTPROJECT_REQUEST_H
#define FIRSTPROJECT_REQUEST_H

#include <string>
#include "iostream"


namespace request {
#define HTTP_VERSION "1.1"

#ifdef _WIN32

#include "winsock2.h"

#elif __linux__
#include "sys/socket.h"
#endif

    class request {

    };
}


#endif //FIRSTPROJECT_REQUEST_H
