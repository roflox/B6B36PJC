#include "expr.hpp"
#include "cmdline.hpp"

#include <iostream>

using namespace std;

void process_expr(std::ostream &os, expr initial_expr, vector<Commands::Command> const &commands) {
    expr e = std::move(initial_expr);
    for (const auto &cmd : commands) {
        using namespace Commands;
        cmd.match(
                [&](Commands::Derive const &derive) {
                    e = e->derive(derive.variable);
                },
                [&](Commands::Simplify const &) {
                    e = e->simplify();
                },
                [&](Commands::Evaluate const &evaluate) {
//                    e = e->evaluate(evaluate.variables);
                    os << e->evaluate(evaluate.variables) << std::endl;
                },
                [&](Commands::Print const &p) {
                    switch (p.format) {
                        case Print::Format::Postfix:
                            os << fmt_expr{e, expr_base::WriteFormat::Postfix} << std::endl;
                            break;
                        case Print::Format::Infix:
                            os << fmt_expr{e, expr_base::WriteFormat::Infix} << std::endl;
                            break;
                        default:
                            os << fmt_expr{e, expr_base::WriteFormat::Prefix} << std::endl;
                            break;
                    }
                }
        );
    }
}

void handle_expr_line(std::ostream &os, std::string const &line, vector<Commands::Command> const &commands) {
    process_expr(os, create_expression_tree(line), commands);
}
