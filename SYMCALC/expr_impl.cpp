#include "expr_impl.hpp"

#include <iostream>
#include <cmath>

namespace exprs {

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
        const number *a = dynamic_cast<number const *>(b.shared_from_this().get());
        if (!a) return false;
        return a->num == num;
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
        const variable *a = dynamic_cast<variable const *>(b.shared_from_this().get());
        if (!a) return false;
        return a->var == var;
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

    bool plus::equals(const expr_base &expr_b) const {
        const plus *expr = dynamic_cast<plus const *>(expr_b.shared_from_this().get());
        if (!expr) return false;
        return expr->a == a && expr->b == b;
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

    bool minus::equals(const expr_base &expr_b) const {
        const minus *expr = dynamic_cast<minus const *>(expr_b.shared_from_this().get());
        if (!expr) return false;
        return expr->a == a && expr->b == b;
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

    bool mult::equals(const expr_base &expr_b) const {
        const mult *expr = dynamic_cast<mult const *>(expr_b.shared_from_this().get());
        if (!expr) return false;
        return expr->a == a && expr->b == b;
    }

    double div::evaluate(const expr_base::variable_map_t &variables) const {
        return a->evaluate(variables) / b->evaluate(variables);
    }

    expr div::derive(const std::string &variable) const {
        return (a->derive(variable) * b - a * b->derive(variable)) / (std::pow(b, expr::number(2)));
    }

    expr div::simplify() const {
        auto x = a->simplify();
        auto y = b->simplify();
        if (x == expr::ZERO and y == expr::ZERO) return shared_from_this();
        if (x == expr::ZERO) return expr::ZERO;
        if (y == expr::ONE) return x;
        return x / y;
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

    bool div::equals(const expr_base &expr_b) const {
        const div *expr = dynamic_cast<div const *>(expr_b.shared_from_this().get());
        if (!expr) return false;
        return expr->a == a && expr->b == b;
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
            case expr_base::WriteFormat::Infix:
                out << "sin(" << fmt_expr{a, fmt} << ")";
                return;
            case expr_base::WriteFormat::Postfix:
                out << fmt_expr{a, fmt} << " sin";
                return;
            default:
                out << "(sin " << fmt_expr{a, fmt} << ")";
                return;
        }
    }

    bool sin::equals(const expr_base &b) const {
        const sin *expr = dynamic_cast<sin const *>(b.shared_from_this().get());
        if (!expr) return false;
        return expr->a == a;
    }

    double cos::evaluate(const expr_base::variable_map_t &variables) const {
        return std::cos(a->evaluate(variables));
    }

    expr cos::derive(const std::string &variable) const {
        return (expr::number(0) - std::sin(a)) * a->derive(variable);
    }

    expr cos::simplify() const {
        return std::cos(a->simplify());
    }

    void cos::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        switch (fmt) {
            case expr_base::WriteFormat::Infix:
                out << "cos(" << fmt_expr{a, fmt} << ")";
                return;
            case expr_base::WriteFormat::Postfix:
                out << fmt_expr{a, fmt} << " cos";
                return;
            default:
                out << "(cos " << fmt_expr{a, fmt} << ")";
                return;
        }
    }

    bool cos::equals(const expr_base &b) const {
        const cos *expr = dynamic_cast<cos const *>(b.shared_from_this().get());
        if (!expr) return false;
        return expr->a == a;
    }

    double log::evaluate(const expr_base::variable_map_t &variables) const {
        return std::log(a->evaluate(variables));
    }

    expr log::derive(const std::string &variable) const {
        return a->derive(variable) / a;
    }

    expr log::simplify() const {
        expr result = a->simplify();
        if (result == expr::ONE) return expr::ZERO;
        return std::log(result);
    }

    void log::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        switch (fmt) {
            case expr_base::WriteFormat::Infix:
                out << "log(" << fmt_expr{a, fmt} << ")";
                return;
            case expr_base::WriteFormat::Postfix:
                out << fmt_expr{a, fmt} << " log";
                return;
            default:
                out << "(log " << fmt_expr{a, fmt} << ")";
                return;
        }
    }

    bool log::equals(const expr_base &b) const {
        const log *expr = dynamic_cast<log const *>(b.shared_from_this().get());
        if (!expr) return false;
        return expr->a == a;
    }

    double pow::evaluate(const expr_base::variable_map_t &variables) const {
        return std::pow(a->evaluate(variables), b->evaluate(variables));
    }

    // vzorec dle testu
    // (* (^ x 2) (+ (/ (* 1 2) x) (* (log x) 0)))
    // (x^2)` = (x^2) * (2/x + log(x)*0)
    // 0 = 2` -> b`
    expr pow::derive(const std::string &variable) const {
        return std::pow(a, b) * ((a->derive(variable) * b) / a + std::log(a) * b->derive(variable));
//        throw std::logic_error("not implemented yet");
//        return expr();
    }

    expr pow::simplify() const {
        auto x = a->simplify();
        auto y = b->simplify();
        if (y == expr::ONE) return x;
        if ((x == expr::ZERO && y == expr::ZERO) || y == expr::ZERO) return expr::ONE;
        if (x == expr::ZERO) return expr::ZERO;
        return std::pow(x, y);
    }

    void pow::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        switch (fmt) {
            case expr_base::WriteFormat::Infix:
                out << "(" << fmt_expr{a, fmt} << " ^ " << fmt_expr{b, fmt} << ")";
                return;
            case expr_base::WriteFormat::Postfix:
                out << fmt_expr{a, fmt} << " " << fmt_expr{b, fmt} << " ^";
                return;
            default:
                out << "(^ " << fmt_expr{a, fmt} << " " << fmt_expr{b, fmt} << ")";
                return;
        }
    }

    bool pow::equals(const expr_base &expr_b) const {
        const pow *expr = dynamic_cast<pow const *>(expr_b.shared_from_this().get());
        if (!expr) return false;
        return expr->a == a && expr->b == b;
    }

} // namespace exprs
