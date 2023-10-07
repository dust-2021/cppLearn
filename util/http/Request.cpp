
#include "Request.h"

#include "string"
#include "unordered_map"

using namespace std;

Header::Header() {
    this->_dict = unordered_map<string, string>{};
}

Header::Header(unordered_map<string, string> &dict) {
    this->_dict = unordered_map(dict);
}

string Header::getValue(string &name) {
    return this->_dict[name];
};

void Header::setValue(string name, string value) {
    this->_dict[std::move(name)] = std::move(value);
}

string Header::yieldRequestHeader() {
    auto result = string();
    for (const auto &pair: this->_dict) {
        result += pair.first + " " + pair.second + "\r\n";
    }
    return result;
}


string Request::agent = "Http 1.1";

Request::Request(std::string &url) {
    this->method = "GET";
    this->url = url;
    this->header = new Header();
}

Request::Request(string &url, Header &header, string &method) {
    this->method = method;
    this->url = url;
    this->header = &header;
}

Request::~Request() {
    if (this->header != nullptr) {
        delete this->header;
        this->header = nullptr;
    }
}