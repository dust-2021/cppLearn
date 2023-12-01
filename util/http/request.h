#ifndef FIRSTPROJECT_REQUEST_H
#define FIRSTPROJECT_REQUEST_H

#include <string>
#include "iostream"


namespace request {
#define HTTP_VERSION "1.1"

    class _socket;

#ifdef _WIN32

#include "winsock2.h"

    WSAData wsaData;

    class _socket {

        sockaddr_in address;


    };

#elif __linux__
#include "sys/socket.h"

    class _socket {

    };
#endif

    class request;

    class response;

    class request {
        _socket soc;

    };
}


#endif //FIRSTPROJECT_REQUEST_H
