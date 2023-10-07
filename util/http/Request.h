

#ifndef FIRSTPROJECT_REQUEST_H
#define FIRSTPROJECT_REQUEST_H


#endif //FIRSTPROJECT_REQUEST_H

# include "string"
#include "unordered_map"
#include "thread"

using namespace std;

class Header {
public:

    Header();

    explicit Header(unordered_map<string, string> &dict);

    string getValue(string &name);

    void setValue(string name, string value);

    string yieldRequestHeader();

private:
    unordered_map<string, string> _dict;


};

class Request {
public:
    // stack
    string method;
    string url;
    static string agent;
    // heap
    Header *header;

    explicit Request(string &url);

    Request(string &url, Header &header, string &method);

    ~Request();

private:
    string request_body;
    static string encode_type;

    void _format_header();
};

