#pragma once

#include "expr.hpp"
#include <iosfwd>

namespace exprs {
    // TODO
    class variable : public expr_base {
        friend class expr;

    public:
        explicit variable(std::string var);

    private:
        std::string var;
    public:
        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        void write(std::ostream &out, WriteFormat fmt) const override;

        bool equals(const expr_base &b) const override;
    };

    class number : public expr_base {
        friend class expr;

    public:
        explicit number(double);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(std::string const &variable) const override;

        expr simplify() const override;

    private:
        void write(std::ostream &out, WriteFormat fmt) const override;

        bool equals(const expr_base &b) const override;

        double num;
    };


    class plus : public expr_base {
        friend class expr;

    public:
        plus(expr, expr);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(std::string const &variable) const override;

        expr simplify() const override;

    private:
        expr a;
        expr b;

        void write(std::ostream &out, WriteFormat fmt) const override;

        bool equals(const expr_base &b) const override;
    };

    class minus : public expr_base {
    public:
        minus(expr, expr);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        expr a;
        expr b;

        void write(std::ostream &out, WriteFormat fmt) const override;

        bool equals(const expr_base &b) const override;
    };

    class multiply : public expr_base {

    private:
    public:
        multiply(expr, expr);

    private:
        expr a;
        expr b;
    };

    class divide : public expr_base {

    public:
        divide(expr a, expr b);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        void write(std::ostream &out, WriteFormat fmt) const override;

        bool equals(const expr_base &b) const override;

    private:
        expr a;
        expr b;
    };
}
