#include "expr_impl.hpp"

#include <iostream>
#include <cmath>
#include <limits>
#include <utility>

namespace exprs {
    // TODO


    plus::plus(expr a, expr b) : a(std::move(a)), b(std::move(b)) {};
    minus::minus(expr a, expr b) : a(std::move(a)), b(std::move(b)) {};
    divide::divide(expr a, expr b) : a(std::move(a)), b(std::move(b)) {}
    multiply::multiply(expr a, expr b) : a(std::move(a)), b(std::move(b)) {}

    double plus::evaluate(const expr_base::variable_map_t &variables) const {
        return 0;
    }

    expr plus::derive(const std::string &variable) const {
        return expr();
    }

    expr plus::simplify() const {
        return expr();
    }

    void plus::write(std::ostream &out, expr_base::WriteFormat fmt) const {

    }

    bool plus::equals(const expr_base &b) const {
        return false;
    }



    number::number(double num) {

    }

    variable::variable(std::string var) {

    }

    double variable::evaluate(const expr_base::variable_map_t &variables) const {
        return 0;
    }

    expr variable::derive(const std::string &variable) const {
        return expr();
    }

    expr variable::simplify() const {
        return expr();
    }

    void variable::write(std::ostream &out, expr_base::WriteFormat fmt) const {

    }

    bool variable::equals(const expr_base &b) const {
        return false;
    }

    double minus::evaluate(const expr_base::variable_map_t &variables) const {
        return 0;
    }

    expr minus::derive(const std::string &variable) const {
        return expr();
    }

    expr minus::simplify() const {
        return expr();
    }

    void minus::write(std::ostream &out, expr_base::WriteFormat fmt) const {

    }

    bool minus::equals(const expr_base &b) const {
        return false;
    }

    minus::minus() {

    }

    double divide::evaluate(const expr_base::variable_map_t &variables) const {
        return 0;
    }

    expr divide::derive(const std::string &variable) const {
        return expr();
    }

    expr divide::simplify() const {
        return expr();
    }

    void divide::write(std::ostream &out, expr_base::WriteFormat fmt) const {

    }

    bool divide::equals(const expr_base &b) const {
        return false;
    }


} // namespace exprs
