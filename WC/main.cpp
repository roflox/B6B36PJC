#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include <ctype.h>

std::map<std::string, long> words_map;

void help() {
    std::cout << "usage: WC [OPTION]...  [FILE]..." << std::endl
              << "options: " << std::endl <<
              "-h, --help            to print help manual" << std::endl <<
              "-s, --separate-files  count words for each file separated" << std::endl;
//              "-t, --total           total count of words" << std::endl;
}

void push_word_to_map(const std::string &word) {
    if (!word.empty()) {
        try {
            words_map.at(word)++;
        } catch (std::out_of_range e) {
            words_map[word] = 1;
        }
    }
}

void print_map() {
    for (const auto &pair:words_map) {
        std::cout << pair.first << " | " << pair.second << std::endl;
    }
}


std::string remove_special_chars(std::string *word) {
    if (word->length() == 0) {
        return *word;
    }
    const char first_char = (*word)[0];
    const char last_char = (*word)[word->length() - 1];
    if (isalnum(first_char) && isalnum(last_char)) {
        return *word;
    }
    if (!isalnum(first_char)) {
        word->erase(0, 1);
    }
    if (!isalnum(last_char)) {
        word->pop_back();
    }
    return remove_special_chars(word);
}

void count_word_for_file(const std::string &file_name) {
    std::ifstream infile(file_name);
    if (infile.is_open()) {
//        long count = 0;
        std::string line;
        std::string word;
        while (std::getline(infile, line)) {
            char *str = const_cast<char *>(line.c_str());
            int state = 0;
            while (*str) {
                if (*str == ' ' || *str == '\n' || *str == '\t') {
                    state = 0;
                } else if (state == 0) {
                    state = 1;
                    push_word_to_map(remove_special_chars(&word));
                    word.clear();
//                    ++count;
                }
                if (state != 0) {
                    word.push_back(*str);
                }
                ++str;
            }
            push_word_to_map(remove_special_chars(&word));
            word.clear();
        }
//        std::cout << count << std::endl;
//total number of words
    } else {
        std::cout << "File " << file_name << " NOT found or could not be opened!" << std::endl;
    }
    infile.close();
}

bool find_arg(std::vector<std::string> &args, const std::string &str) {
    return std::find(args.begin(), args.end(), str) != args.end();
}

int main(int argc, char *argv[]) {
    bool separate = false;
    if (argc < 2) {
        help();
    } else {
        std::vector<std::string> arguments(argv + 1, argv + argc);
        if (find_arg(arguments, "--help") || find_arg(arguments, "-h")) {
            help();
            return 0;
        }
        if (find_arg(arguments, "--separate-files")) {
            arguments.erase(std::find(arguments.begin(), arguments.end(), "--separate-files"));
            separate = true;
        }
        if (find_arg(arguments, "-s")) {
            arguments.erase(std::find(arguments.begin(), arguments.end(), "-s"));
            separate = true;
        }
        if (!separate) {
            for (const auto &arg: arguments) {
                count_word_for_file(arg);
            }
            print_map();
        } else {
            for (const auto &arg: arguments) {
                count_word_for_file(arg);
                std::cout << "Words for file " << arg << std::endl;
                print_map();
                std::cout << std::endl;
                words_map.clear();
            }
        }

    }

    return 0;
}
