#pragma once

#include <iostream>
#include <string>
#include "typedefs.hpp"

enum class SEVERITY {
    DEBUG,
    INFO,
    ERR
};


class Logger final {
public:
    Logger() = default;

    static void log(SEVERITY type, std::string message) {
        auto severity = parseSeverity(type);
        std::cout << "[" << severity << "]" << ": " << message;
    }

private:
    static const char *parseSeverity(SEVERITY type) {
        switch (type) {
            case SEVERITY::DEBUG:
                return "DEBUG";
                break;

            case SEVERITY::INFO:
                return "INFO";
                break;

            case SEVERITY::ERR:
                return "ERROR";
                break;
            default:
                return "UNDEFINED";
                break;
        }
    }

};
