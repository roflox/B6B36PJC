#include "expr.hpp"
#include "expr_impl.hpp"
#include <stdexcept>
#include "sstream"
#include "tokenizer.hpp"
#include <memory>
#include <deque>
#include <stack>
#include <iostream>

const expr expr::ZERO = expr::number(0.0);
const expr expr::ONE = expr::number(1.0);

// TODO: overloaded operators +, -, *, /, functions pow, log, sin, cos,
//       expr::number, expr::variable, operator==, operator<<,
//       create_expression_tree

bool isFunction(const Token &t) {
    if (t.id != TokenId::Identifier)
        return false;
    else return t.identifier == "cos" or t.identifier == "sin" or t.identifier == "log";
}

expr create_expression_tree(const std::string &expression) {
    std::cout << expression << std::endl;
    auto stringStream = std::stringstream(expression);
    auto tokenizer = Tokenizer(stringStream);
    std::deque<Token> outputQueue;
    std::stack<Token> operatorStack;
    Token previousToken = Token(TokenId::End);
    auto token = tokenizer.next();
    while (token.id != TokenId::End) {
        switch (token.id) {
            case TokenId::Number:
                if (previousToken.id == TokenId::Number) {
                    throw parse_error("missing operator between two numbers");
                } else if (previousToken.id == TokenId::RParen) {
                    throw parse_error("missing operator between right parenthesis");
                }
                outputQueue.push_back(token);
                break;
            case TokenId::Identifier:
                if (isFunction(previousToken)) {
                    throw parse_error("missing left parenthesis after another identifier");
                } else if (previousToken.id == TokenId::RParen) {
                    throw parse_error("missing operator after right parenthesis");
                }
                if (!isFunction(token)) {
                    outputQueue.push_back(token);
                } else {
                    operatorStack.push(token);
                }
                break;
            case TokenId::Plus:
            case TokenId::Minus:
            case TokenId::Multiply:
            case TokenId::Divide:
            case TokenId::Power:
                if (isFunction(previousToken)) {
                    throw parse_error("missing left parenthesis after identifier");
                }
                while (!operatorStack.empty()) {
                    auto top = operatorStack.top();
                    if (isFunction(top) or (top.is_binary_op() and (
                            (top.op_precedence() > token.op_precedence() or
                             (top.op_precedence() == token.op_precedence() and
                              token.associativity() == Associativity::Left)) and
                            top.id != TokenId::LParen))) {
                        outputQueue.push_back(operatorStack.top());
                        operatorStack.pop();
                        continue;
                    }
                    break;
                }
                operatorStack.push(token);
                break;
            case TokenId::LParen:
                operatorStack.push(token);
                break;
            case TokenId::RParen:
                while (!operatorStack.empty()) {
                    if (operatorStack.top().id != TokenId::LParen) {
                        outputQueue.push_back(operatorStack.top());
                        operatorStack.pop();
                        continue;
                    }
                    break;
                }
                if (!operatorStack.empty()) {
                    if (operatorStack.top().id == TokenId::LParen) {
                        operatorStack.pop();
                    }
                }
                break;
        }
        previousToken = token;
        token = tokenizer.next();
    }
    while (!operatorStack.empty()) {
        outputQueue.push_back(operatorStack.top());
        operatorStack.pop();
    }
//    throw std::logic_error("not implemented");
    std::deque<expr> expressions;
//    whi(const auto &token: outputQueue) {
    for (const auto &queueToken: outputQueue) {
        if (queueToken.id == TokenId::Number) {
            expressions.push_back(expr::number(queueToken.number));
        } else if (TokenId::Identifier == queueToken.id) {
            if (isFunction(queueToken)) {
                if (expression.empty()) {
                    throw parse_error("function needs an argument");
                }
                auto a = expressions.back();
                expressions.pop_back();
                if (queueToken.identifier == "sin") {
                    expressions.push_back(sin(a));
                } else if (queueToken.identifier == "cos") {
                    expressions.push_back(cos(a));
                } else if (queueToken.identifier == "log") {
                    expressions.push_back(log(a));
                } else {
                    throw tokenize_error("unknown function");
                }
            } else {
                expressions.push_back(expr::variable(queueToken.identifier));
            }
        } else {
            if (expressions.size() < 2)
                throw parse_error("operator need at least two expressions");
            auto b = expressions.back();
            expressions.pop_back();
            auto a = expressions.back();
            expressions.pop_back();
            if (queueToken.id == TokenId::Plus) {
                expressions.push_back(a + b);
            } else if (queueToken.id == TokenId::Minus) {
                expressions.push_back(a - b);
            } else if (queueToken.id == TokenId::Multiply) {
                expressions.push_back(a * b);
            } else if (queueToken.id == TokenId::Divide) {
                expressions.push_back(a / b);
            } else if (queueToken.id == TokenId::Power) {
                expressions.push_back(pow(a, b));
            }

        }
    }
    auto res = expressions.front();
    return res;
}

bool operator==(const expr &a, const expr &b) {
    return a->equals(*b->shared_from_this());
}

std::ostream &operator<<(std::ostream &os, const expr &e) {
    e->write(os, expr::WriteFormat::Prefix);
    return os;
}

std::ostream &operator<<(std::ostream &os, const fmt_expr &f) {
    f.e->write(os, f.fmt);
    return os;
}

expr expr::number(double n) {
    return std::make_shared<exprs::number>(n);
}

expr expr::variable(std::string name) {
    return std::make_shared<exprs::variable>(exprs::variable(std::move(name)));
}

expr operator+(expr a, expr b) {
    return std::make_shared<exprs::plus>(exprs::plus(std::move(a), std::move(b)));
}

expr operator-(expr a, expr b) {
    return std::make_shared<exprs::minus>(exprs::minus(std::move(a), std::move(b)));
}

expr operator*(expr a, expr b) {
    return std::make_shared<exprs::mult>(exprs::mult(std::move(a), std::move(b)));
}

expr operator/(expr a, expr b) {
    return std::make_shared<exprs::div>(exprs::div(std::move(a), std::move(b)));
}

expr sin(expr a) {
    return std::make_shared<exprs::sin>(exprs::sin(std::move(a)));
}

expr cos(expr a) {
    return std::make_shared<exprs::cos>(exprs::cos(std::move(a)));
}

expr log(expr a) {
    return std::make_shared<exprs::log>(exprs::log(std::move(a)));
}

expr pow(expr a, expr b) {
    return std::make_shared<exprs::pow>(exprs::pow(std::move(a), std::move(b)));
}