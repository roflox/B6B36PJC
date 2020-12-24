#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

//std::string getOsName() {
//#ifdef _WIN32
//    return "Windows 32-bit";
//#elif _WIN64
//    return "Windows 64-bit";
//#elif __APPLE__ || __MACH__
//    return "Mac OSX";
//#elif __linux__
//    return "Linux";
//#elif __FreeBSD__
//    return "FreeBSD";
//#elif __unix || __unix__
//    return "Unix";
//#else
//    return "Other";
//#endif
//}

void help() {
    std::cout << "usage: WC [file1] [file2] [...]" << std::endl;
}

void count_word_for_file(const std::string &file_name) {
    std::ifstream infile(file_name);
    if (infile.is_open()) {
        long count = 0;
        std::string line;
        while (std::getline(infile, line)) {
            char *str = const_cast<char *>(line.c_str());
            int state = 0;
            while (*str) {
                if (*str == ' ' || *str == '\n' || *str == '\t')
                    state = 0;
                else if (state == 0) {
                    state = 1;
                    ++count;
                }
                ++str;
            }
        }
        std::cout << count << "  " << file_name << std::endl;
    }
    infile.close();
}

bool find_arg(std::vector<std::string> &args, const std::string &str) {
    return std::find(args.begin(), args.end(), str) != args.end();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        help();
    } else {
        std::vector<std::string> arguments(argv + 1, argv + argc);
        if (find_arg(arguments, "--help") || find_arg(arguments, "-h")) {
            help();
        } else {
            for (const auto &arg: arguments) {
                count_word_for_file(arg);
            }
        }
    }


//    auto os = getOsName();
//    if (os != "Linux") {
//
//    }
//    if (os == "Linux") {
//        if (1 < argc) {
//            std::vector<std::string> arguments(argv + 1, argv + argc);
//            for (const auto &arg: arguments) {
//                std::string command = "wc -w ";
//                command.append(arg);
//                system(command.c_str());
//            }
//        }
//    }
    return 0;
}
