#pragma once

#include "expr.hpp"
#include <iosfwd>

namespace exprs {
    // TODO
    class variable : public expr_base {
        friend class expr;

    public:
        explicit variable(std::string);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        void write(std::ostream &out, WriteFormat fmt) const override;

        bool equals(const expr_base &b) const override;

    private:
        std::string var;

    };

    class number : public expr_base {
        friend class expr;

    public:
        explicit number(double);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        void write(std::ostream &out, WriteFormat fmt) const override;

        bool equals(const expr_base &b) const override;

    private:
        double num;
    };


    class plus : public expr_base {
        friend class expr;

    public:
        plus(expr, expr);

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

    class minus : public expr_base {
    public:
        minus(expr, expr);

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

    class mult : public expr_base {

    private:
    public:
        mult(expr, expr);

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

    class div : public expr_base {

    public:
        div(expr, expr);

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

    class sin : public expr_base {
    public:
        sin(expr);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        void write(std::ostream &out, WriteFormat fmt) const override;

        bool equals(const expr_base &b) const override;

    private:
        expr a;
    };

    class cos : public expr_base {
    public:
        cos(expr a);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        void write(std::ostream &out, WriteFormat fmt) const override;

        bool equals(const expr_base &b) const override;

    private:

    private:
        expr a;
    };

    class log : public expr_base {
    public:
        log(expr);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        void write(std::ostream &out, WriteFormat fmt) const override;

        bool equals(const expr_base &b) const override;

    private:
        expr a;
    };

    class pow : public expr_base {
    public:
        pow(expr, expr);

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
