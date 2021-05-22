#include "math.hpp"

math::math() {
    operators = {"+", "-", "/", "*", "^"};
    leftAssociative = {"+", "-", "*", "/"};
    precedenceMap = {
            {"^", 30},
            {"/", 20},
            {"*", 20},
            {"+", 10},
            {"-", 10},
    };

    functions = {"sin", "cos", "tan", "max", "min"};
}


math &math::shuntingYard(const std::string &input) {
    std::string token;
    for (int i = 0; i < input.size(); ++i) {
        const auto c = std::string(1, input[i]);

        // If there's more than one digit
        if (isNumber(c)) {
            digitQ.push(parseNumber(input, i));
        } else if (isOperator(c)) {
            if (operatorStack.empty()) {
                goto push_op;
            }
            while (!operatorStack.empty()) {
                if ((precedenceMap[operatorStack.top()] > precedenceMap[c]) ||
                    (precedenceMap[operatorStack.top()] == precedenceMap[c] && isLeftAssociative(c)) &&
                    (operatorStack.top() != "(")) {
                    digitQ.push(operatorStack.top());
                    operatorStack.pop();
                } else {
                    goto push_op;
                }
            }
            push_op:
            operatorStack.push(c);
        }
        if (c == "(") {
            operatorStack.push(c);
        }
        if (c == ")") {
            while (operatorStack.top() != "(" && !operatorStack.empty()) {
                digitQ.push(operatorStack.top());
                operatorStack.pop();
            }
            if (operatorStack.top() == "(") {
                operatorStack.pop();
            } else {
                throw std::runtime_error("no matching '(' parenthesis");
            }
        }
        if (isFunction(token)) {
            operatorStack.push(token);
            token.clear();
        }
        if (isalpha(input[i])) {
            token += c;
        }
    }

    while (!operatorStack.empty()) {
        digitQ.push(operatorStack.top());
        operatorStack.pop();
    }

    return *this;

}


// Terribly unoptimized code
float math::evaluatePostfix() {
    if (digitQ.empty()) {
        throw std::runtime_error("be sure to run shuntingYard first");
    }

    q_condition:
    if (!digitQ.empty()) {
        while (isNumber(digitQ.front())) {
            digits.push(std::stof(digitQ.front()));
            digitQ.pop();
        }
        // parsing an operator
        if (digitQ.front() == "+") {
            auto rhs = digits.top();
            digits.pop();
            auto lhs = digits.top();
            digits.pop();
            digits.push(lhs + rhs);
            digitQ.pop();
        } else if (digitQ.front() == "-") {
            auto rhs = digits.top();
            digits.pop();
            auto lhs = digits.top();
            digits.pop();
            digits.push(lhs - rhs);
            digitQ.pop();
        } else if (digitQ.front() == "*") {
            auto rhs = digits.top();
            digits.pop();
            auto lhs = digits.top();
            digits.pop();
            digits.push(lhs * rhs);
            digitQ.pop();
        } else if (digitQ.front() == "/") {
            auto rhs = digits.top();
            digits.pop();
            auto lhs = digits.top();
            assert(rhs != 0 && "blowing up to infinity");
            digits.pop();
            digits.push(lhs * rhs);
            digitQ.pop();
        } else if (digitQ.front() == "sin") {
            auto arg = digits.top();
            digits.pop();
            digits.push(std::sin(arg));
            digitQ.pop();
        } else if (digitQ.front() == "cos") {
            auto arg = digits.top();
            digits.pop();
            digits.push(std::cos(arg));
            digitQ.pop();
        } else if (digitQ.front() == "tan") {
            auto arg = digits.top();
            digits.pop();
            digits.push(std::tan(arg));
            digitQ.pop();
        } else if (digitQ.front() == "max") {
            auto arg1 = digits.top();
            digits.pop();
            auto arg2 = digits.top();
            digits.pop();
            digits.push(std::max(arg1, arg2));
            digitQ.pop();
        } else if (digitQ.front() == "min") {
            auto arg1 = digits.top();
            digits.pop();
            auto arg2 = digits.top();
            digits.pop();
            digits.push(std::min(arg1, arg2));
            digitQ.pop();
        }
        goto q_condition;
    }
    return digits.top();
}


