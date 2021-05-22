#pragma once

#include "../../core/typedefs.hpp"
#include "../../core/logger.hpp"


struct request_data {
    std::string key;
    std::string value;
};

struct request_header {
    std::string method;
    std::string path;
    std::vector <request_data> req_data;
    std::string protocol;
    ui16 majVer;
    ui16 minVer;
};

class request {
public:
    explicit request(const std::string &req_str) :
            req_str_(req_str) {};

    bool isGood();

    inline request_header header() const { return req_header_; }

private:
    void parse();

    void parseRequestBody(const std::string& body);

    std::map<std::string, std::string>
    splitBodyParams(const std::string& body);

    void consume(char c);

private:
    std::string req_str_;
    request_header req_header_{};
};