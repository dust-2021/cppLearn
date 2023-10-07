

#ifndef FIRSTPROJECT_RESPONSE_H
#define FIRSTPROJECT_RESPONSE_H

#include <utility>

#include "string"
#include "unordered_map"

using namespace std;

class Response {
public:
    string method;
    string url;
    string host;
    unsigned int port;

    unordered_map<string, string> headers;
    string context;
    string _origin_text;

    Response(string &text);

    unordered_map<string, string> parse_headers();

    unordered_map<string, string> parse_json();

};


#endif //FIRSTPROJECT_RESPONSE_H
