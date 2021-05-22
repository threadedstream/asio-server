#include "request.hpp"


namespace valid_data {

    std::unordered_set<std::string> methods = {
            "GET", "POST", "PUT", "DELETE", "HEAD"
    };
}

/*
*/
void request::parseRequestBody(const std::string &body) {
    auto map = splitBodyParams(body);

    for (const auto&[key, value] : map) {
        req_header_.req_data.emplace_back(request_data{key, value});
    }
}

void request::parse() {
    int i = 0;
    char currC = 0;
    //parsing request method
    while (req_str_[i] != ' ') {
        currC = req_str_[i];
        req_header_.method += currC;
        i++;
    }
    i++;

    //parsing path
    while (req_str_[i] != ' ') {
        currC = req_str_[i];
        req_header_.path += currC;
        i++;
    }
    i++;

    //parsing protocol;
    while (req_str_[i] != '/') {
        currC = req_str_[i];
        req_header_.protocol += currC;
        i++;
    }
    i++;

    req_header_.majVer = req_str_[i] - '0';
    req_header_.minVer = req_str_[i + 2] - '0';

    auto bodyIdx = req_str_.find_last_of("\r\n") + 1;

    //parsing request body
    parseRequestBody(req_str_.substr(bodyIdx));
}


bool request::isGood() {
    parse();
    if (valid_data::methods.find(req_header_.method.c_str()) != valid_data::methods.end()
        && req_header_.protocol == "HTTP") {
        return true;
    } else {
        Logger::log(SEVERITY::DEBUG,
                    std::to_string(valid_data::methods.find(req_header_.method.c_str()) != valid_data::methods.end()));
        Logger::log(SEVERITY::DEBUG, std::to_string(req_header_.protocol == "HTTP"));
        return false;
    }
}

std::map<std::string, std::string>
request::splitBodyParams(const std::string &body) {
    std::map<std::string, std::string> output;
    std::string token;
    const auto split = [](const std::string &str) -> std::pair<std::string, std::string> {
        auto idx = str.find_first_of('=');
        return std::pair(str.substr(0, idx), str.substr(idx + 1));
    };

    for (int i = 0; i < body.size(); ++i) {
        if (body[i] == '&') {
            const auto pair = split(token);
            output.emplace(pair.first, pair.second);
            token.clear();
        } else {
            token += body[i];
        }
    }
}


void request::consume(char c) {
}
