#include "cmdline.hpp" // parse_command
#include "app.hpp" // handle_expr_line
#include "iostream"

int main(int argc, char *argv[]) {

    std::vector<Commands::Command> commands;

    commands.reserve(argc - 1);
    for (int i = 0; i < argc - 1; i++) {
        commands.push_back(parse_command(argv[i + 1]));
    }

    std::string lajn;

    while (std::getline(std::cin, lajn)) {
        try {
            handle_expr_line(std::cout, lajn, commands);
        } catch (const std::exception &e) {
            std::cout << "nefunguje to " << std::endl;
        }
    }
}
