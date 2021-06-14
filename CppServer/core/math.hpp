#pragma once

#include <iostream>
#include <regex>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <cmath>
#include <cassert>

class math {
public:
    math &
    shuntingYard(const std::string &input);

    float dot(const std::string& vec1, const std::string& vec2);
    std::string cross(const std::string& vec1, const std::string& vec2);

    math();

    float evaluatePostfix();

private:
    __attribute__((always_inline)) bool
    isOperator(const std::string &tok) {
        return operators.find(tok) != operators.end();
    }

    __attribute__((always_inline)) bool
    isLeftAssociative(const std::string &tok) {
        return leftAssociative.find(tok) != leftAssociative.end();
    }

    __attribute__((always_inline)) bool
    isNumber(const std::string &tok) {
        std::regex numRegex("^\\d+$");
        return std::regex_match(tok, numRegex);
    }

    __attribute__((always_inline)) bool
    isFunction(const std::string &tok){
        return std::find(functions.begin(), functions.end(), tok) != functions.end();
    }

    [[nodiscard]] std::string
    parseNumber(const std::string &str, int &pos) {
        std::string num;
        while (pos < str.size() && isdigit(str[pos])) {
            num += str[pos];
            pos++;
        }
        pos--;
        return num;
    }

private:
    // fields
    std::queue<std::string> digitQ;
    std::stack<std::string> operatorStack;
    std::set<std::string> operators;
    std::set<std::string> leftAssociative;
    std::map<std::string, uint8_t> precedenceMap;
    std::set<std::string> functions;

    std::stack<float> digits;
};

