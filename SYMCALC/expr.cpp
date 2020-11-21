#include "expr.hpp"
#include "expr_impl.hpp"
#include <stdexcept>
#include "sstream"
#include "tokenizer.hpp"
#include <memory>
#include <utility>
#include <deque>
#include <vector>
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

bool isOperator(const Token &t) {
    return t.id == TokenId::Minus or t.id == TokenId::Plus or t.id == TokenId::Multiply or
           t.id == TokenId::Divide or
           t.id == TokenId::Power;

}

expr create_expression_tree(const std::string &expression) {
    // TODO
    auto stringStream = std::stringstream(expression);
    auto tokenizer = Tokenizer(stringStream);

    std::deque<Token> outputQueue;
    std::stack<Token> operatorStack;
    Token previousToken = Token(TokenId::End);
    while (true) {
        auto token = tokenizer.next();
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
                if(!isFunction(token)){
                    outputQueue.push_back(token);
                }else{
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
                    if (isOperator(operatorStack.top()) and
                        (operatorStack.top().op_precedence() > token.op_precedence() or
                         (operatorStack.top().op_precedence() == token.op_precedence() and
                          token.associativity() == Associativity::Left)) and
                        operatorStack.top().id != TokenId::LParen)
                    {
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
                    } else {
                        break;
                    }
                }
                if (!operatorStack.empty()) {
                    if (operatorStack.top().id == TokenId::LParen) {
                        operatorStack.pop();
                    }
                }
                break;
            default:
                break;
        }
        if (token.id == TokenId::End)
            break;
        previousToken = token;
    }
    while (!operatorStack.empty()) {
        outputQueue.push_back(operatorStack.top());
        operatorStack.pop();
    }
//    throw std::logic_error("not implemented");
    std::deque<expr> expressions;
    for (const auto &token: outputQueue) {
        if (token.id == TokenId::Number) {
            expressions.push_back(expr::number(token.number));
        } else if (TokenId::Identifier == token.id) {
            if (isFunction(token)) {
                auto a = expressions.back();
                expressions.pop_back();
                if (token.identifier == "sin") {
                    expressions.push_back(sin(a));
                } else if (token.identifier == "cos") {
                    expressions.push_back(cos(a));
                } else if (token.identifier == "log") {
                    expressions.push_back(log(a));
                } else {
                    throw tokenize_error("unknown function");
                }
            } else {
                expressions.push_back(expr::variable(token.identifier));
            }
        } else {
            if (expressions.size() < 2)
                throw parse_error("operator need at least two expressions");
            auto b = expressions.back();
            expressions.pop_back();
            auto a = expressions.back();
            expressions.pop_back();
            if (token.id == TokenId::Plus) {
                expressions.push_back(a + b);
            } else if (token.id == TokenId::Minus) {
                expressions.push_back(a - b);
            } else if (token.id == TokenId::Multiply) {
                expressions.push_back(a * b);
            } else if (token.id == TokenId::Divide) {
                expressions.push_back(a / b);
            } else if (token.id == TokenId::Power) {
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