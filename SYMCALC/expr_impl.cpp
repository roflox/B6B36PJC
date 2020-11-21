#include "expr_impl.hpp"

#include <iostream>
#include <cmath>
#include <limits>
#include <utility>

namespace exprs {
    // TODO

    number::number(double number) : num(number) {}

    variable::variable(std::string a) : var(std::move(a)) {}

    plus::plus(expr a, expr b) : a(std::move(a)), b(std::move(b)) {}

    minus::minus(expr a, expr b) : a(std::move(a)), b(std::move(b)) {}

    mult::mult(expr a, expr b) : a(std::move(a)), b(std::move(b)) {}

    div::div(expr a, expr b) : a(std::move(a)), b(std::move(b)) {}

    sin::sin(expr a) : a(std::move(a)) {}

    cos::cos(expr a) : a(std::move(a)) {}

    log::log(expr a) : a(std::move(a)) {}

    pow::pow(expr a, expr b) : a(std::move(a)), b(std::move(b)) {}

    double number::evaluate(const expr_base::variable_map_t &variables) const {
        return num;
    }

    expr number::derive(const std::string &variable) const {
        return expr::ZERO;
    }

    expr number::simplify() const {
        return shared_from_this();
    }

    void number::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << num;
    }

    bool number::equals(const expr_base &b) const {
        return false;
    }

    double variable::evaluate(const expr_base::variable_map_t &variables) const {
        if (variables.find(var) != variables.end()) {
            //todo variable does not exist
            return 0;
        }
        return variables.at(var);
    }

    expr variable::derive(const std::string &variable) const {
        if (variable == var)
            return expr::ONE;
        return expr::ZERO;
    }

    expr variable::simplify() const {
        return shared_from_this();
    }

    void variable::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << var;
    }

    bool variable::equals(const expr_base &b) const {
        return false;
    }

    double plus::evaluate(const expr_base::variable_map_t &variables) const {
        return a->evaluate(variables) + b->evaluate(variables);
    }

    expr plus::derive(const std::string &variable) const {
        return a->derive(variable) + b->derive(variable);
    }

    expr plus::simplify() const {
        auto x = a->simplify();
        auto y = b->simplify();
        if (x == expr::ZERO)
            return y;
        if (y == expr::ZERO) return x;
        return x + y;
    }

    void plus::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        switch (fmt) {
            case expr_base::WriteFormat::Postfix:
                out << fmt_expr{a, fmt} << " " << fmt_expr{b, fmt} << " +";
                return;
            case expr_base::WriteFormat::Infix:
                out << "(" << fmt_expr{a, fmt} << " + " << fmt_expr{b, fmt} << ")";
                return;
            default:
                out << "(+ " << fmt_expr{a, fmt} << " " << fmt_expr{b, fmt} << ")";
                return;
        }
    }

    bool plus::equals(const expr_base &b) const {
        return false;
    }

    double minus::evaluate(const expr_base::variable_map_t &variables) const {
        return a->evaluate(variables) - b->evaluate(variables);
    }

    expr minus::derive(const std::string &variables) const {
        return a->derive(variables) - b->derive(variables);
    }

    expr minus::simplify() const {
        auto x = a->simplify();
        auto y = b->simplify();
        if (x == expr::ZERO)
            return y;
        if (y == expr::ZERO) return x;
        return x - y;
    }

    void minus::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        switch (fmt) {
            case expr_base::WriteFormat::Postfix:
                out << fmt_expr{a, fmt} << " " << fmt_expr{b, fmt} << " -";
                return;
            case expr_base::WriteFormat::Infix:
                out << "(" << fmt_expr{a, fmt} << " - " << fmt_expr{b, fmt} << ")";
                return;
            default:
                out << "(- " << fmt_expr{a, fmt} << " " << fmt_expr{b, fmt} << ")";
                return;
        }
    }

    bool minus::equals(const expr_base &b) const {
        return false;
    }

    double mult::evaluate(const expr_base::variable_map_t &variables) const {
        return a->evaluate(variables) * b->evaluate(variables);
    }

    expr mult::derive(const std::string &variable) const {
        return a->derive(variable) * b + a * b->derive(variable);
    }

    expr mult::simplify() const {
        auto x = a->simplify();
        auto y = b->simplify();
        if (x == expr::ZERO || y == expr::ZERO) return expr::ZERO;
        if (x == expr::ONE) return y;
        if (y == expr::ONE) return x;
        return x * y;
    }

    void mult::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        switch (fmt) {
            case expr_base::WriteFormat::Postfix:
                out << fmt_expr{a, fmt} << " " << fmt_expr{b, fmt} << " *";
                return;
            case expr_base::WriteFormat::Infix:
                out << "(" << fmt_expr{a, fmt} << " * " << fmt_expr{b, fmt} << ")";
                return;
            default:
                out << "(* " << fmt_expr{a, fmt} << " " << fmt_expr{b, fmt} << ")";
                return;
        }
    }

    bool mult::equals(const expr_base &b) const {
        return false;
    }

    double div::evaluate(const expr_base::variable_map_t &variables) const {
        return a->evaluate(variables) / b->evaluate(variables);
    }

    expr div::derive(const std::string &variable) const {
        return (a->derive(variable) * b - a * b->derive(variable) ) / (std::pow(b, expr::number(2)));
    }

    expr div::simplify() const {
        auto x = a->simplify();
        auto y = b->simplify();
        if (x == expr::ZERO || y == expr::ZERO) return shared_from_this();
        if (x == expr::ZERO) return expr::ZERO;
        if (y == expr::ONE) return x;
        return x * y;
    }


    void div::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        switch (fmt) {
            case expr_base::WriteFormat::Postfix:
                out << fmt_expr{a, fmt} << " " << fmt_expr{b, fmt} << " /";
                return;
            case expr_base::WriteFormat::Infix:
                out << "(" << fmt_expr{a, fmt} << " / " << fmt_expr{b, fmt} << ")";
                return;
            default:
                out << "(/ " << fmt_expr{a, fmt} << " " << fmt_expr{b, fmt} << ")";
                return;
        }
    }

    bool div::equals(const expr_base &b) const {
        return false;
    }

    double sin::evaluate(const expr_base::variable_map_t &variables) const {
        return std::sin(a->evaluate(variables));
    }

    expr sin::derive(const std::string &variable) const {
        return std::cos(a) * a->derive(variable);
    }

    expr sin::simplify() const {
        return std::sin(a->simplify());
    }

    void sin::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        switch (fmt) {
            case expr_base::WriteFormat::Postfix:
                out << "sin(" << fmt_expr{a, fmt} << ")";
                return;
            case expr_base::WriteFormat::Infix:
                out << fmt_expr{a, fmt} << " sin";
                return;
            default:
                out << "(sin " << fmt_expr{a, fmt} << ")";
                return;
        }
    }

    bool sin::equals(const expr_base &b) const {
        return false;
    }

    double cos::evaluate(const expr_base::variable_map_t &variables) const {
        return std::cos(a->evaluate(variables));
    }

    expr cos::derive(const std::string &variable) const {
        return (expr::number(-1) * std::sin(a)) * a->derive(variable);
    }

    expr cos::simplify() const {
        return std::cos(a->simplify());
    }

    void cos::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        switch (fmt) {
            case expr_base::WriteFormat::Postfix:
                out << "cos(" << fmt_expr{a, fmt} << ")";
                return;
            case expr_base::WriteFormat::Infix:
                out << fmt_expr{a, fmt} << " cos";
                return;
            default:
                out << "(cos " << fmt_expr{a, fmt} << ")";
                return;
        }
    }

    bool cos::equals(const expr_base &b) const {
        return false;
    }

    double log::evaluate(const expr_base::variable_map_t &variables) const {
        return std::log(a->evaluate(variables));
    }

    expr log::derive(const std::string &variable) const {
        return a->derive(variable) / a;
    }

    expr log::simplify() const {
        return expr();
    }

    void log::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        switch (fmt) {
            case expr_base::WriteFormat::Postfix:
                out << "log(" << fmt_expr{a, fmt} << ")";
                return;
            case expr_base::WriteFormat::Infix:
                out << fmt_expr{a, fmt} << " log";
                return;
            default:
                out << "(log " << fmt_expr{a, fmt} << ")";
                return;
        }
    }

    bool log::equals(const expr_base &b) const {
        return false;
    }

    double pow::evaluate(const expr_base::variable_map_t &variables) const {
        return std::pow(a->evaluate(variables), b->evaluate(variables));
    }

    expr pow::derive(const std::string &variable) const {
        return expr();
    }

    expr pow::simplify() const {
        return expr();
    }

    void pow::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        switch (fmt) {
            case expr_base::WriteFormat::Postfix:
                out << "(" << fmt_expr{a, fmt} << " ^ " << fmt_expr{b, fmt} << ")";
                return;
            case expr_base::WriteFormat::Infix:
                out << fmt_expr{a, fmt} << " " << fmt_expr{b, fmt} << " ^";
                return;
            default:
                out << "(^ " << fmt_expr{a, fmt} << " " << fmt_expr{b, fmt} << ")";
                return;
        }
    }

    bool pow::equals(const expr_base &b) const {
        return false;
    }

} // namespace exprs
