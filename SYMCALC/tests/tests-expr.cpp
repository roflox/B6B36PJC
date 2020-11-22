#ifdef RUNNER
# include "runner.hpp"
#else
# include "expr.hpp"
#endif

#include "tests.hpp"
#include "cmdline.hpp"

#include "catch.hpp"
#include <sstream>


namespace {
#ifdef RUNNER
// The exception type is lost, so we do not check it
# define CHECK_THROWS_MAYBEAS(expr, exc_type) CHECK_THROWS(expr)

    void check_lines_one(std::pair<int, std::vector<std::string>> const &code_lines) {
        int errcode = code_lines.first;
        const auto &lines = code_lines.second;

        INFO("Program output lines: {\"" << fmt_range(lines, "\", \"") << "\"}");
        CHECK(errcode == 0);
        CHECK(lines.size() == 1);
        if (lines.empty())
            return;
        const auto &line = lines[0];

        // throw an exception if the output line starts with "!"
        if (!line.empty() && line[0] == '!')
            throw std::runtime_error(line);
    }
    using namespace Commands;
    std::string parse_and_write(std::string input, Commands::Print::Format fmt = Commands::Print::Format::Default) {
        auto code_lines = process({Print{fmt}}, {std::move(input)});
        check_lines_one(code_lines);
        return code_lines.second.at(0);
    }
    std::string test_simplify(std::string input) {
        auto code_lines = process({Simplify{}, Print{}}, {std::move(input)});
        check_lines_one(code_lines);
        return code_lines.second.at(0);
    }
    std::string test_derive(std::string input, const char *var) {
        auto code_lines = process({Derive{var}, Print{}}, {std::move(input)});
        check_lines_one(code_lines);
        return code_lines.second.at(0);
    }
    std::string test_derive_simplify(std::string input, const char *var) {
        auto code_lines = process({Derive{var}, Simplify{}, Print{}}, {std::move(input)});
        check_lines_one(code_lines);
        return code_lines.second.at(0);
    }
    double test_evaluate(std::string input, std::map<std::string, double> const &vm) {
        auto code_lines = process({Evaluate{vm}}, {std::move(input)});
        check_lines_one(code_lines);
        return std::stod(code_lines.second.at(0));
    }
#else
# define CHECK_THROWS_MAYBEAS(expr, exc_type) CHECK_THROWS_AS(expr, exc_type)
    std::string to_string(expr e) {
        std::stringstream ss;
        ss << e;
        return ss.str();
    }
    std::string parse_and_write(std::string input) {
        expr e = create_expression_tree(input);
        return to_string(e);
    }
    std::string test_simplify(std::string input) {
        expr e = create_expression_tree(input);
        e = e->simplify();
        return to_string(e);
    }
    std::string test_derive(std::string input, const char *var) {
        expr e = create_expression_tree(input);
        e = e->derive(var);
        return to_string(e);
    }
    std::string test_derive_simplify(std::string input, const char *var) {
        expr e = create_expression_tree(input);
        e = e->derive(var);
        e = e->simplify();
        return to_string(e);
    }
    double test_evaluate(std::string input, expr::variable_map_t const &vm) {
        expr e = create_expression_tree(input);
        return e->evaluate(vm);
    }
#endif
}

struct TestDeriveData {
    const char *input;
    const char *variable;
    const char *expected;
};

struct TestEvaluateData {
    const char *input;
    std::map<std::string, double> variables;
    double expected;
};


TEST_CASE("Parsing and writing", "[parse_write][valid]") {
    static const TestData s_test_data[] = {
        { "1",                "1" },
        { "12.34",            "12.34" },

        // If you read & write numbers using iostreams with default settings,
        // this is what you get, but we don't test it.
         { "1.26e6",           "1.26e+06" },
         { "1.26e-6",          "1.26e-06" },

        { "variable",         "variable" },
        { "sin(x)",           "(sin x)" },
        { "cos(x)",           "(cos x)" },
        { "log(x)",           "(log x)" },
        { "x+1",           "(+ x 1)" },
        { "1+1",              "(+ 1 1)" },
        { "1-1",              "(- 1 1)" },
        { "1*1",              "(* 1 1)" },
        { "1/1",              "(/ 1 1)" },
        { "1^1",              "(^ 1 1)" },

        // should matter only for evaluate, not here
        { "log(0)",           "(log 0)" },

        // whitespace
        { "sin ( x ) ",       "(sin x)" },
        { " 1   \t + 1  ",    "(+ 1 1)" },

        // brackets
        { "(1)", "1" },
        { "(1+(((sin(x)))))", "(+ 1 (sin x))" },

        // precedence
        { "1+2*3",            "(+ 1 (* 2 3))" },
        { "1+2*3*4",          "(+ 1 (* (* 2 3) 4))" },
        { "1*2^3",            "(* 1 (^ 2 3))" },
        { "1*2^3^4",          "(* 1 (^ 2 (^ 3 4)))" },

        { "(1+2)*3*4",        "(* (* (+ 1 2) 3) 4)" },
        { "(1*2)^3^4",        "(^ (* 1 2) (^ 3 4))" },

        { "2^sin(1+1)",       "(^ 2 (sin (+ 1 1)))" },


        { " 31.47 +95.13", "(+ 31.47 95.13)" },
        { "12.83 / x", "(/ 12.83 x)" },
        { "def+ y", "(+ def y)" },
        { "log(y)", "(log y)" },
        { "4.17^ abc ", "(^ 4.17 abc)" },
        { "24.32 / 12.67", "(/ 24.32 12.67)" },
        { "x - 98.23", "(- x 98.23)" },
        { " z - def", "(- z def)" },
        { "abc/ 41.23", "(/ abc 41.23)" },
        { "29.62^z", "(^ 29.62 z)" },
        { "21.44/ 51.75 ", "(/ 21.44 51.75)" },
        { " def+def", "(+ def def)" },
        { "def + 89.53 ", "(+ def 89.53)" },
        { " z+ 87.10 ", "(+ z 87.1)" },
        { " z +39.26", "(+ z 39.26)" },
        { "y- 36.05", "(- y 36.05)" },
        { "59.38 / y", "(/ 59.38 y)" },
        { "def+ 68.24", "(+ def 68.24)" },
        { " 72.28 ^ 64.85", "(^ 72.28 64.85)" },
        { "log(9.92)", "(log 9.92)" },
        { "65.09 *45.96", "(* 65.09 45.96)" },
        { " z+ ( 50.37)", "(+ z 50.37)" },
        { "z ^def", "(^ z def)" },
        { "11.62 * 91.00", "(* 11.62 91)" },
        { " 41.61+ 10.22", "(+ 41.61 10.22)" },
        { " y *20.77", "(* y 20.77)" },
        { " 58.67- 15.22", "(- 58.67 15.22)" },
        { "59.00 -78.03 ", "(- 59 78.03)" },
        { "log(z)", "(log z)" },
        { " log(47.73) ", "(log 47.73)" },
        { "def +def", "(+ def def)" },
        { " 76.73+ abc", "(+ 76.73 abc)" },
        { " log(41.65)", "(log 41.65)" },
        { "57.24 -x", "(- 57.24 x)" },
        { "13.77 + 46.11 ", "(+ 13.77 46.11)" },
        { " 88.12 *def", "(* 88.12 def)" },
        { "41.56 ^ y", "(^ 41.56 y)" },
        { "53.85/ 37.60", "(/ 53.85 37.6)" },
        { " 29.18/ abc", "(/ 29.18 abc)" },
        { "10.25 +z", "(+ 10.25 z)" },
        { "10.56 *59.31", "(* 10.56 59.31)" },
        { " abc + 84.03", "(+ abc 84.03)" },
        { "44.25 + 42.74 ", "(+ 44.25 42.74)" },
        { "60.66 * 78.32", "(* 60.66 78.32)" },
        { "93.78 - 52.82 ", "(- 93.78 52.82)" },
        { "abc* 36.38", "(* abc 36.38)" },
        { " abc / 6.70 ", "(/ abc 6.7)" },
        { "log(2.34)", "(log 2.34)" },
        { "y + y", "(+ y y)" },
        { " x +z", "(+ x z)" },
        { "sin(69.39^37.63)", "(sin (^ 69.39 37.63))" },
        { "98.43 - 20.49 -61.75+ 37.00 ", "(+ (- (- 98.43 20.49) 61.75) 37)" },
        { " 9.67 + 59.63 -log(37.31) ", "(- (+ 9.67 59.63) (log 37.31))" },
        { "( 53.43/ 59.10) ^log(38.15) ", "(^ (/ 53.43 59.1) (log 38.15))" },
        { " abc* 21.08/def", "(/ (* abc 21.08) def)" },
        { " 13.17 ^ 58.60 -z", "(- (^ 13.17 58.6) z)" },
        { "abc + ( 21.38) + 74.08 / 21.72", "(+ (+ abc 21.38) (/ 74.08 21.72))" },
        { "40.21+ 40.49 + 42.60 + 27.37", "(+ (+ (+ 40.21 40.49) 42.6) 27.37)" },
        { "log(68.51 + 64.54)", "(log (+ 68.51 64.54))" },
        { "x - 69.06 + sin(52.48) ", "(+ (- x 69.06) (sin 52.48))" },
        { " log(75.57 / 19.51)", "(log (/ 75.57 19.51))" },
        { "abc -31.13-13.87+15.05 ", "(+ (- (- abc 31.13) 13.87) 15.05)" },
        { "log(53.23 ^z) ", "(log (^ 53.23 z))" },
        { " 59.04 / 54.27 /log(75.67)", "(/ (/ 59.04 54.27) (log 75.67))" },
        { "log(23.89 - z) ", "(log (- 23.89 z))" },
        { "y / 73.49 +46.05 / abc", "(+ (/ y 73.49) (/ 46.05 abc))" },
        { "sin(y)* 71.44 / 72.90", "(/ (* (sin y) 71.44) 72.9)" },
        { "sin(abc /37.79)", "(sin (/ abc 37.79))" },
        { "log(abc) +54.75", "(+ (log abc) 54.75)" },
        { "y ^ 66.63 - 26.92 -39.86 ", "(- (- (^ y 66.63) 26.92) 39.86)" },
        { " 63.01 -x - y - 24.98 ", "(- (- (- 63.01 x) y) 24.98)" },
        { "97.13*99.92 *log(abc) ", "(* (* 97.13 99.92) (log abc))" },
        { "98.17- sin(78.50)", "(- 98.17 (sin 78.5))" },
        { "27.26 ^ 20.00+ log(24.01)", "(+ (^ 27.26 20) (log 24.01))" },
        { " log(57.86)- 19.61 +27.93", "(+ (- (log 57.86) 19.61) 27.93)" },
        { " log(67.51 ^ 39.55)", "(log (^ 67.51 39.55))" },
        { " 20.08 +37.14 *57.86", "(+ 20.08 (* 37.14 57.86))" },
        { "log(49.80 -x)", "(log (- 49.8 x))" },
        { "( 44.52 - 37.87)* ( abc -x)", "(* (- 44.52 37.87) (- abc x))" },
        { "log(52.98 * y)", "(log (* 52.98 y))" },
        { " 96.37 ^37.48 / sin(90.16)", "(/ (^ 96.37 37.48) (sin 90.16))" },
        { "log(z ^ 74.24)", "(log (^ z 74.24))" },
        { "log(y - 86.14)", "(log (- y 86.14))" },
        { "abc ^ 42.93 * y* 25.84", "(* (* (^ abc 42.93) y) 25.84)" },
        { "19.67 + 15.72 - 22.14 + 99.28", "(+ (- (+ 19.67 15.72) 22.14) 99.28)" },
        { "46.54 + 68.91 - 14.99 ", "(- (+ 46.54 68.91) 14.99)" },
        { " y / 48.81 + 15.15 / 82.72", "(+ (/ y 48.81) (/ 15.15 82.72))" },
        { "log(log(53.14)) ", "(log (log 53.14))" },
        { "z-49.50 - 2.57/ 29.86", "(- (- z 49.5) (/ 2.57 29.86))" },
        { " 49.74 - z- abc-72.84", "(- (- (- 49.74 z) abc) 72.84)" },
        { " 24.36 - 44.48 - 93.35 * x", "(- (- 24.36 44.48) (* 93.35 x))" },
        { " z* z+ 88.47 / def", "(+ (* z z) (/ 88.47 def))" },
        { "sin(def+ 48.23)", "(sin (+ def 48.23))" },
        { "92.60+ y- def- 15.73", "(- (- (+ 92.6 y) def) 15.73)" },
        { "log(97.59) + 13.05 ^60.44", "(+ (log 97.59) (^ 13.05 60.44))" },
        { "sin(log(abc))", "(sin (log abc))" },
        { " 59.98 * y- 25.14", "(- (* 59.98 y) 25.14)" },
        { "log(43.11-90.46) ", "(log (- 43.11 90.46))" },
        { " log(93.99 +z) ", "(log (+ 93.99 z))" },
        { " 31.59 / 98.04*log(y)", "(* (/ 31.59 98.04) (log y))" },
        { " log(78.13) * y * x + 19.31 +z+ x/ 71.62", "(+ (+ (+ (* (* (log 78.13) y) x) 19.31) z) (/ x 71.62))" },
        { "log(log(55.36* 25.80)) ", "(log (log (* 55.36 25.8)))" },
        { "log(log(11.02 ^x))", "(log (log (^ 11.02 x)))" },
        { "( 27.53 -46.41)*( 52.87 / y+abc)", "(* (- 27.53 46.41) (+ (/ 52.87 y) abc))" },
        { "sin(log(log(x)))", "(sin (log (log x)))" },
        { "abc - z - def - log(98.06) *21.29 ^ 65.79 ", "(- (- (- abc z) def) (* (log 98.06) (^ 21.29 65.79)))" },
        { "log(z ^17.90) +def + 23.80 -abc / 97.36 ", "(- (+ (+ (log (^ z 17.9)) def) 23.8) (/ abc 97.36))" },
        { " 34.21 / abc + 61.60 + y- 24.99*61.72/sin(y)", "(- (+ (+ (/ 34.21 abc) 61.6) y) (/ (* 24.99 61.72) (sin y)))" },
        { " ( 46.72- 67.48) *86.72^ z + log(z) ^80.91 ^ abc", "(+ (* (- 46.72 67.48) (^ 86.72 z)) (^ (log z) (^ 80.91 abc)))" },
        { "log(94.98 + 92.32 + 77.62)", "(log (+ (+ 94.98 92.32) 77.62))" },
        { "log(91.04) - log(log(11.73))", "(- (log 91.04) (log (log 11.73)))" },
        { "sin(39.70+ 56.31 - 21.06)", "(sin (- (+ 39.7 56.31) 21.06))" },
        { "log(( 19.61 + 81.09) ^ ( 44.21 + x))", "(log (^ (+ 19.61 81.09) (+ 44.21 x)))" },
        { "log(log(93.32) - 49.91 / x)", "(log (- (log 93.32) (/ 49.91 x)))" },
        { "log(83.48^ 30.67* 12.50)", "(log (* (^ 83.48 30.67) 12.5))" },
        { "1.68 + z+32.47^ y +13.38 - 24.10+ 3.97", "(+ (- (+ (+ (+ 1.68 z) (^ 32.47 y)) 13.38) 24.1) 3.97)" },
        { " log(y* def /def)", "(log (/ (* y def) def))" },
        { " log(64.30 -60.74)", "(log (- 64.3 60.74))" },
        { "log(54.58 + z)-( 2.10 + 4.26) * x* 57.62", "(- (log (+ 54.58 z)) (* (* (+ 2.1 4.26) x) 57.62))" },
        { "sin(def + 46.34 - 8.11 * x) ", "(sin (- (+ def 46.34) (* 8.11 x)))" },
        { "sin(4.40 + def) + ( 22.43+90.67)", "(+ (sin (+ 4.4 def)) (+ 22.43 90.67))" },
        { " sin(def* abc +def- y) ", "(sin (- (+ (* def abc) def) y))" },
        { " 92.02 /( y+30.22) + 86.59/ 13.37 ", "(+ (/ 92.02 (+ y 30.22)) (/ 86.59 13.37))" },
        { "42.80 - abc+57.27 + 67.99- log(x) + sin(71.96)", "(+ (- (+ (+ (- 42.8 abc) 57.27) 67.99) (log x)) (sin 71.96))" },
        { "def - 68.86- 78.13 + 75.08 +64.62+ abc + z +y", "(+ (+ (+ (+ (+ (- (- def 68.86) 78.13) 75.08) 64.62) abc) z) y)" },
        { " log(log(x) - z)", "(log (- (log x) z))" },
        { "log(88.86- 24.92 ^ 98.78)", "(log (- 88.86 (^ 24.92 98.78)))" },
        { "log(x * 89.87+71.42 -51.50)", "(log (- (+ (* x 89.87) 71.42) 51.5))" },
        { " sin(90.54) + 15.35", "(+ (sin 90.54) 15.35)" },
        { " 31.56 +z +10.50- 16.32 +log(73.13 * y) ", "(+ (- (+ (+ 31.56 z) 10.5) 16.32) (log (* 73.13 y)))" },
        { "sin(74.78 + y+ 42.95- 36.79)", "(sin (- (+ (+ 74.78 y) 42.95) 36.79))" },
        { " abc * 35.11 +log(64.33) -def /def * 59.06 /85.41", "(- (+ (* abc 35.11) (log 64.33)) (/ (* (/ def def) 59.06) 85.41))" },
        { " log(x - 85.49 +def)", "(log (+ (- x 85.49) def))" },
        { " log(17.48 - 65.53- 67.02) ", "(log (- (- 17.48 65.53) 67.02))" },
        { "log(log(abc+ 48.55)) ", "(log (log (+ abc 48.55)))" },
        { "sin(z + 26.61 +z*30.56)", "(sin (+ (+ z 26.61) (* z 30.56)))" },
        { "log(5.47/82.81+ 86.92 * 76.70)", "(log (+ (/ 5.47 82.81) (* 86.92 76.7)))" },
        { "29.36 / 13.72 + ( 72.99) -9.50+32.02/ y + 2.62+ z ", "(+ (+ (+ (- (+ (/ 29.36 13.72) 72.99) 9.5) (/ 32.02 y)) 2.62) z)" },
        { " sin(y *27.92 /78.10)", "(sin (/ (* y 27.92) 78.1))" },
        { " sin(sin(def))", "(sin (sin def))" },
        { "y +def-23.65 - log(x+4.63)", "(- (- (+ y def) 23.65) (log (+ x 4.63)))" },
        { "sin(82.15 - 51.91) + 61.44 / abc* 77.78 * 62.64", "(+ (sin (- 82.15 51.91)) (* (* (/ 61.44 abc) 77.78) 62.64))" },
        { "log(log(32.11) +9.36 - 47.85) ", "(log (- (+ (log 32.11) 9.36) 47.85))" },
        { "91.27 /y ^ 80.10 + def/x* 95.51", "(+ (/ 91.27 (^ y 80.1)) (* (/ def x) 95.51))" },
        { "sin(z/ 96.03 * 90.76^ x)", "(sin (* (/ z 96.03) (^ 90.76 x)))" },
        { "sin(log(y)* 1.32) ", "(sin (* (log y) 1.32))" },
        { " log(67.22 +def) + log(55.39 +61.81)", "(+ (log (+ 67.22 def)) (log (+ 55.39 61.81)))" },
        { "54.57+ z - 24.96- z *15.36+70.78 -96.34 ", "(- (+ (- (- (+ 54.57 z) 24.96) (* z 15.36)) 70.78) 96.34)" },
        { "log(y + 22.11-def - 34.97)", "(log (- (- (+ y 22.11) def) 34.97))" },
        { "sin(log(sin(71.93)))", "(sin (log (sin 71.93)))" },
        { " log(log(sin(65.66) -54.01))", "(log (log (- (sin 65.66) 54.01)))" },
        { "log(99.13/y)- 75.34 -38.83+ 92.15 + 22.71 -abc / 11.98+ def/ 50.91 / log(51.57)", "(+ (- (+ (+ (- (- (log (/ 99.13 y)) 75.34) 38.83) 92.15) 22.71) (/ abc 11.98)) (/ (/ def 50.91) (log 51.57)))" },
        { "21.70/35.52 *99.82 /74.04 / 28.50 / log(97.84) +48.60/ def *abc -88.15", "(- (+ (/ (/ (/ (* (/ 21.7 35.52) 99.82) 74.04) 28.5) (log 97.84)) (* (/ 48.6 def) abc)) 88.15)" },
        { "13.64^ def /51.50 *x -92.45 /def-39.90*1.90+def/def * log(98.06) -64.63/ z", "(- (+ (- (- (* (/ (^ 13.64 def) 51.5) x) (/ 92.45 def)) (* 39.9 1.9)) (* (/ def def) (log 98.06))) (/ 64.63 z))" },
        { " log(95.43/73.31 +99.59) - sin(47.70^ def / abc)", "(- (log (+ (/ 95.43 73.31) 99.59)) (sin (/ (^ 47.7 def) abc)))" },
        { "log(sin(80.29 / def)+ log(57.16 - 75.67))", "(log (+ (sin (/ 80.29 def)) (log (- 57.16 75.67))))" },
        { "log(log(17.38) -z*y+ 44.99 ^ 59.61 * y ^ z)", "(log (+ (- (log 17.38) (* z y)) (* (^ 44.99 59.61) (^ y z))))" },
        { "log(sin(sin(1.58))) + 49.25 +80.65 + 22.89+def / def", "(+ (+ (+ (+ (log (sin (sin 1.58))) 49.25) 80.65) 22.89) (/ def def))" },
        { " log(log(def +7.34)+ 27.04 *4.87 + 50.41* 61.49)", "(log (+ (+ (log (+ def 7.34)) (* 27.04 4.87)) (* 50.41 61.49)))" },
        { " 10.54 / log(78.56 * 7.07*y ^39.48)", "(/ 10.54 (log (* (* 78.56 7.07) (^ y 39.48))))" },
        { "sin(49.83+ sin(22.71) - log(93.01))", "(sin (- (+ 49.83 (sin 22.71)) (log 93.01)))" },
        { "10.88 * 58.39 + 14.31 -y+ log(abc * 80.91) - log(8.74 -57.98 + 17.23 + 82.14)", "(- (+ (- (+ (* 10.88 58.39) 14.31) y) (log (* abc 80.91))) (log (+ (+ (- 8.74 57.98) 17.23) 82.14)))" },
        { " log(log(y) +65.42+ 61.85* abc)", "(log (+ (+ (log y) 65.42) (* 61.85 abc)))" },
        { " sin(16.36) +log(13.71)- 57.81 * 16.32 +35.29 + x + log(x * 69.10 + 5.42-0.95)", "(+ (+ (+ (- (+ (sin 16.36) (log 13.71)) (* 57.81 16.32)) 35.29) x) (log (- (+ (* x 69.1) 5.42) 0.95)))" },
        { "def +73.84/abc/ log(99.14) /82.48/ 37.13", "(+ def (/ (/ (/ (/ 73.84 abc) (log 99.14)) 82.48) 37.13))" },
        { " log(21.58 + 31.55^64.18 + z ^( x +z))", "(log (+ (+ 21.58 (^ 31.55 64.18)) (^ z (+ x z))))" },
        { " 38.64+ 97.77- 88.96 -log(90.21) +log(log(99.07- 68.15))", "(+ (- (- (+ 38.64 97.77) 88.96) (log 90.21)) (log (log (- 99.07 68.15))))" },
        { "sin(def/( x+ 54.90)) -24.79+ 36.17+ 80.83-45.96+ z", "(+ (- (+ (+ (- (sin (/ def (+ x 54.9))) 24.79) 36.17) 80.83) 45.96) z)" },
        { "y+54.36 - 79.03 -log(66.94) +sin(49.82) + sin(x) ^x - abc", "(- (+ (+ (- (- (+ y 54.36) 79.03) (log 66.94)) (sin 49.82)) (^ (sin x) x)) abc)" },
        { " log(log(53.50 /x - 31.06 ^ x))", "(log (log (- (/ 53.5 x) (^ 31.06 x))))" },
        { "log(log(36.18 * 10.57)/ log(x + def))", "(log (/ (log (* 36.18 10.57)) (log (+ x def))))" },
        { "log(39.80 -5.74 - 97.39 * 12.38+ log(x))", "(log (+ (- (- 39.8 5.74) (* 97.39 12.38)) (log x)))" },
        { " log(sin(z + abc) ^ log(z *71.43))", "(log (^ (sin (+ z abc)) (log (* z 71.43))))" },
        { "sin(log(log(abc)) +sin(def)/ 24.76) ", "(sin (+ (log (log abc)) (/ (sin def) 24.76)))" },
        { "sin(log(x)*48.10 - def-log(27.84)) ", "(sin (- (- (* (log x) 48.1) def) (log 27.84)))" },
        { "log(sin(log(44.46 + 16.84)))", "(log (sin (log (+ 44.46 16.84))))" },
        { "log(def / abc+81.13 ^96.47 + z * ( z - def))", "(log (+ (+ (/ def abc) (^ 81.13 96.47)) (* z (- z def))))" },
        { "log(log(4.96 - 94.95-2.80/ 11.64))", "(log (log (- (- 4.96 94.95) (/ 2.8 11.64))))" },
        { " log(log(def-( 63.44))-31.60 + 57.74 - 93.66 / 38.02) ", "(log (- (+ (- (log (- def 63.44)) 31.6) 57.74) (/ 93.66 38.02)))" },
        { "log(59.19 +def - z * 58.68-( 98.37+ 79.28) / 14.69) ", "(log (- (- (+ 59.19 def) (* z 58.68)) (/ (+ 98.37 79.28) 14.69)))" },
        { "log(log(sin(y)) / abc * 57.34 /22.81)", "(log (/ (* (/ (log (sin y)) abc) 57.34) 22.81))" },
        { " log(log(69.21 ^ y- 91.62)) ", "(log (log (- (^ 69.21 y) 91.62)))" },
        { "log(12.82 ^ x + z/ y)/ log(93.22 ^ def)", "(/ (log (+ (^ 12.82 x) (/ z y))) (log (^ 93.22 def)))" },
        { "64.52 /69.79 -47.41 ^ 72.35+ 66.49 ^ 2.86 +40.92 - 87.43 + 47.96+ 34.67/ z", "(+ (+ (- (+ (+ (- (/ 64.52 69.79) (^ 47.41 72.35)) (^ 66.49 2.86)) 40.92) 87.43) 47.96) (/ 34.67 z))" },
        { "log(log(log(def) / abc *0.14)) ", "(log (log (* (/ (log def) abc) 0.14)))" },
        { "sin(sin(x+18.96))", "(sin (sin (+ x 18.96)))" },
        { " log(74.97 - 74.59 -x -def ^ 68.28+ sin(34.53))", "(log (+ (- (- (- 74.97 74.59) x) (^ def 68.28)) (sin 34.53)))" },
        { "log(( 51.54 - 48.52 +72.73 + 50.21) /log(z)^ 77.01) ", "(log (/ (+ (+ (- 51.54 48.52) 72.73) 50.21) (^ (log z) 77.01)))" },
        { " ( z * 96.59 + 65.63 ^ 31.74 - abc + y - sin(abc)) ^ ( 54.57^z +( 8.16) + 90.38 + x)", "(^ (- (+ (- (+ (* z 96.59) (^ 65.63 31.74)) abc) y) (sin abc)) (+ (+ (+ (^ 54.57 z) 8.16) 90.38) x))" },
        { "85.22 ^40.26+sin(log(23.33)) -log(log(x*3.31))", "(- (+ (^ 85.22 40.26) (sin (log 23.33))) (log (log (* x 3.31))))" },
        { "log(91.68+ 54.20+ y +log(sin(78.54)))", "(log (+ (+ (+ 91.68 54.2) y) (log (sin 78.54))))" },
        { " sin(29.23)+log(55.88 - y) -2.01-( x- abc)/67.32 *36.58", "(- (- (+ (sin 29.23) (log (- 55.88 y))) 2.01) (* (/ (- x abc) 67.32) 36.58))" },
        { "log(log(31.10)- sin(def) + sin(5.30 + def))", "(log (+ (- (log 31.1) (sin def)) (sin (+ 5.3 def))))" },
        { " log(16.08- 45.24) - log(82.08^ 24.18)", "(- (log (- 16.08 45.24)) (log (^ 82.08 24.18)))" },
        { "log(29.71 -sin(z) + 65.79)", "(log (+ (- 29.71 (sin z)) 65.79))" },
        { "sin(log(85.56* 92.73* log(9.59)))", "(sin (log (* (* 85.56 92.73) (log 9.59))))" },
        { " log(log(sin(50.62)) - 45.90 + y- 82.91)", "(log (- (+ (- (log (sin 50.62)) 45.9) y) 82.91))" },
        { "sin(71.38 * 87.98)^ log(x + 29.91)+log(5.28)+ 92.73 +46.04* 80.14", "(+ (+ (+ (^ (sin (* 71.38 87.98)) (log (+ x 29.91))) (log 5.28)) 92.73) (* 46.04 80.14))" },
        { "97.64 - 29.83 -98.55+42.71 - log(68.66) - sin(96.33) +97.03 /52.85- x -x - 99.18 + 4.76 * 32.45", "(+ (- (- (- (+ (- (- (+ (- (- 97.64 29.83) 98.55) 42.71) (log 68.66)) (sin 96.33)) (/ 97.03 52.85)) x) x) 99.18) (* 4.76 32.45))" },
        { "log(sin(11.40*26.19+ 51.79)) ", "(log (sin (+ (* 11.4 26.19) 51.79)))" },
        { "log(x+31.71 - log(71.15) -log(53.28 - def +42.26))", "(log (- (- (+ x 31.71) (log 71.15)) (log (+ (- 53.28 def) 42.26))))" },
        { "sin(log(y)) ", "(sin (log y))" },
        { " log(sin(57.56+ 32.30))", "(log (sin (+ 57.56 32.3)))" },
        { " log(log(sin(23.60)))", "(log (log (sin 23.6)))" },
        { " log(40.27- 64.89/ 51.25 ^ 8.99 / ( z -60.84))", "(log (- 40.27 (/ (/ 64.89 (^ 51.25 8.99)) (- z 60.84))))" },
        { "log(sin(log(41.23))* 83.60 * x) ", "(log (* (* (sin (log 41.23)) 83.6) x))" },
        { "log(log(y+ 58.28 *x+ 42.79))", "(log (log (+ (+ y (* 58.28 x)) 42.79)))" },
        { "log(log(71.55/ y /6.69 /x+ 91.52 * 73.59))", "(log (log (+ (/ (/ (/ 71.55 y) 6.69) x) (* 91.52 73.59))))" },
        { "64.79 +def - 12.73- def+70.63+ abc - 17.88 - 72.75 *70.48 + ( 87.40)-log(log(26.80 + def +40.75 * 6.42)) ", "(- (+ (- (- (+ (+ (- (- (+ 64.79 def) 12.73) def) 70.63) abc) 17.88) (* 72.75 70.48)) 87.4) (log (log (+ (+ 26.8 def) (* 40.75 6.42)))))" },
        { "log(sin(77.93)) ", "(log (sin 77.93))" },
        { " sin(log(y/ sin(abc)) /21.53)", "(sin (/ (log (/ y (sin abc))) 21.53))" },
        { "log(log(sin(14.46 + abc + y * 12.81))) ", "(log (log (sin (+ (+ 14.46 abc) (* y 12.81)))))" },
        { "sin(log(17.52 + 66.82/def))", "(sin (log (+ 17.52 (/ 66.82 def))))" },
        { "log(sin(log(63.96 /94.37)))", "(log (sin (log (/ 63.96 94.37))))" },
        { "sin(def- sin(log(61.36) - log(def)))", "(sin (- def (sin (- (log 61.36) (log def)))))" },
        { "log(log(9.49* 98.60/13.39))", "(log (log (/ (* 9.49 98.6) 13.39)))" },
        { "x+55.47* 44.68+ sin(def)- log(72.37 / 67.39) ^43.55 ^( 53.00- 26.16) - sin(log(sin(79.44))* 52.43)", "(- (- (+ (+ x (* 55.47 44.68)) (sin def)) (^ (log (/ 72.37 67.39)) (^ 43.55 (- 53 26.16)))) (sin (* (log (sin 79.44)) 52.43)))" },
        { "sin(( 14.18)+ x + 89.74 + y +x +90.75+ 11.49 - 7.87+def)", "(sin (+ (- (+ (+ (+ (+ (+ (+ 14.18 x) 89.74) y) x) 90.75) 11.49) 7.87) def))" },
        { "sin(log(log(abc) - x + sin(y * y)))", "(sin (log (+ (- (log abc) x) (sin (* y y)))))" },
        { "z + z / def + log(39.12) /( 92.50 -y+x +def) ", "(+ (+ z (/ z def)) (/ (log 39.12) (+ (+ (- 92.5 y) x) def)))" },
        { "sin(log(35.60 -46.19 + 64.34)/log(15.28+52.65)/ 69.92) ", "(sin (/ (/ (log (+ (- 35.6 46.19) 64.34)) (log (+ 15.28 52.65))) 69.92))" },
        { " sin(log(x* log(def))) ", "(sin (log (* x (log def))))" },
        { "log(z + 74.50^ 29.24 - 82.98 + 89.61)+sin(sin(37.08- x+x))", "(+ (log (+ (- (+ z (^ 74.5 29.24)) 82.98) 89.61)) (sin (sin (+ (- 37.08 x) x))))" },
        { " ( log(log(49.10) *log(38.28^def))) + sin(def)", "(+ (log (* (log 49.1) (log (^ 38.28 def)))) (sin def))" },
        { "sin(log(sin(y / x ^ 0.46)))", "(sin (log (sin (/ y (^ x 0.46)))))" },
        { "log(57.31/ 22.76 / 24.65 / 67.34/ log(y -x))", "(log (/ (/ (/ (/ 57.31 22.76) 24.65) 67.34) (log (- y x))))" },
        { " log(85.92+y - 32.13-def- sin(sin(0.68 + 15.44))) ", "(log (- (- (- (+ 85.92 y) 32.13) def) (sin (sin (+ 0.68 15.44)))))" },
        { "sin(log(x+ x+ 87.17 + abc))", "(sin (log (+ (+ (+ x x) 87.17) abc)))" },
        { "sin(log(z -log(z)- abc -62.62))", "(sin (log (- (- (- z (log z)) abc) 62.62)))" },
        { "7.01 + 80.90 + 70.68 + y- y -sin(17.68+ 51.76)+ 15.14 /y + log(abc +88.77- ( abc))- 93.76 * sin(60.52) /def ", "(- (+ (+ (- (- (+ (+ (+ 7.01 80.9) 70.68) y) y) (sin (+ 17.68 51.76))) (/ 15.14 y)) (log (- (+ abc 88.77) abc))) (/ (* 93.76 (sin 60.52)) def))" },
        { "sin(log(abc) - x -59.67 +14.73 +log(62.45))", "(sin (+ (+ (- (- (log abc) x) 59.67) 14.73) (log 62.45)))" },
        { " sin(53.86) ", "(sin 53.86)" },
        { " log(sin(abc/x) / 77.49 /85.79* 16.42* log(20.68)^21.63)", "(log (* (* (/ (/ (sin (/ abc x)) 77.49) 85.79) 16.42) (^ (log 20.68) 21.63)))" },
        { "x / 79.01*81.18 * z / 95.02 - sin(z)- 52.63 * 78.51 /29.70 /sin(abc - 83.91)+ 61.55/ 25.05 * def- 25.54 + 28.17", "(+ (- (+ (- (- (/ (* (* (/ x 79.01) 81.18) z) 95.02) (sin z)) (/ (/ (* 52.63 78.51) 29.7) (sin (- abc 83.91)))) (* (/ 61.55 25.05) def)) 25.54) 28.17)" },
        { "15.71* 27.37 - 67.26 - log(x) - log(12.47)+log(log(20.51) ^ log(98.29))", "(+ (- (- (- (* 15.71 27.37) 67.26) (log x)) (log 12.47)) (log (^ (log 20.51) (log 98.29))))" },
        { " sin(log(z+98.34 +18.59 - 55.55)) ", "(sin (log (- (+ (+ z 98.34) 18.59) 55.55)))" },
        { "sin(log(log(x - sin(x))))", "(sin (log (log (- x (sin x)))))" },
        { "sin(( sin(23.75) -abc * 58.73- 94.97/ 65.86) /( log(90.60) + 12.14 / z)) ", "(sin (/ (- (- (sin 23.75) (* abc 58.73)) (/ 94.97 65.86)) (+ (log 90.6) (/ 12.14 z))))" },
        { " log(log(log(20.56 + 20.16)))", "(log (log (log (+ 20.56 20.16))))" },
        { " log(sin(def))^ log(48.84 /90.61- 35.15 ^ 70.62-abc / 38.19 /y)", "(^ (log (sin def)) (log (- (- (/ 48.84 90.61) (^ 35.15 70.62)) (/ (/ abc 38.19) y))))" },
        { "log(6.80 * 8.48)- sin(87.87* 54.78- 87.83 *y)+log(def + z) + log(abc) + 90.55", "(+ (+ (+ (- (log (* 6.8 8.48)) (sin (- (* 87.87 54.78) (* 87.83 y)))) (log (+ def z))) (log abc)) 90.55)" },
        { " sin(79.71 + 83.41 + 72.70 - 25.79 / 71.28 - 80.06)", "(sin (- (- (+ (+ 79.71 83.41) 72.7) (/ 25.79 71.28)) 80.06))" },
        { "y +log(def -38.11) + 78.07*42.11 +97.92*x - sin(29.20 + 67.22 +def + 4.14 -25.87) ", "(- (+ (+ (+ y (log (- def 38.11))) (* 78.07 42.11)) (* 97.92 x)) (sin (- (+ (+ (+ 29.2 67.22) def) 4.14) 25.87)))" },
        { " sin(80.43) ", "(sin 80.43)" },
        { "log(81.13 *8.34 +57.62* 44.57 / 93.75- log(24.41 -z)- log(sin(z))) ", "(log (- (- (+ (* 81.13 8.34) (/ (* 57.62 44.57) 93.75)) (log (- 24.41 z))) (log (sin z))))" },
        { " sin(y) ", "(sin y)" },
        { " log(sin(50.09 + 12.06 -y+ 32.09 +z))", "(log (sin (+ (+ (- (+ 50.09 12.06) y) 32.09) z)))" },
        { "sin(def) /36.92 /( ( 18.10) - log(sin(x))) - log(85.73 /( x) ^ 66.82+ z/ 16.66)", "(- (/ (/ (sin def) 36.92) (- 18.1 (log (sin x)))) (log (+ (/ 85.73 (^ x 66.82)) (/ z 16.66))))" },
        { "log(log(sin(45.25-x) ^ ( 31.00 - abc-z+ def)))", "(log (log (^ (sin (- 45.25 x)) (+ (- (- 31 abc) z) def))))" },
        { " log(log(67.22) -abc^56.83- x)+ log(97.41-log(6.21)) ^ sin(9.98)", "(+ (log (- (- (log 67.22) (^ abc 56.83)) x)) (^ (log (- 97.41 (log 6.21))) (sin 9.98)))" }
    };
    for (const auto &test : s_test_data) {
        SECTION(test.input) {
            INFO("For input: " << test.input);
            REQUIRE(parse_and_write(test.input) == test.expected);
        }
    }
}

TEST_CASE("Simplify", "[simplify][valid]") {
    static const TestData s_test_data[] = {
        // rules
        { "0+x",           "x" },
        { "x+0",           "x" },
        { "x-0",           "x" },
        { "0*x",           "0" },
        { "x*0",           "0" },
        { "1*x",           "x" },
        { "x*1",           "x" },
        { "0/0",           "(/ 0 0)" },
        { "0/x",           "0" },
        { "x/1",           "x" },
        { "x^1",           "x" },
        { "0^0",           "1" },
        { "x^0",           "1" },
        { "0^x",           "0" },
        { "log(1)",        "0" },

        // depth
        { "0*x + x*0",     "0" },
        { "0*x - x*0",     "0" },
        { "(0*x) * (x*0)", "0" },
        { "(0*x) / (1+0)", "0" },
        { "(0*x) ^ (x*0)", "1" },
        { "log(1*1)",      "0" },
        { "sin(1*1)",      "(sin 1)" },
        { "cos(1*1)",      "(cos 1)" },

        // indempotence
        { "42",            "42" },
        { "xxx",           "xxx" },
        { "1+x",           "(+ 1 x)" },
        { "1-x",           "(- 1 x)" },
        { "2*x",           "(* 2 x)" },
        { "2/x",           "(/ 2 x)" },
        { "2^x",           "(^ 2 x)" },
        { "sin(1)",        "(sin 1)" },
        { "cos(1)",        "(cos 1)" },
        { "log(2)",        "(log 2)" },

        { "2*x + x*2",     "(+ (* 2 x) (* x 2))" },
        { "2*x - x*2",     "(- (* 2 x) (* x 2))" },
        { "(2*x) * (x*2)", "(* (* 2 x) (* x 2))" },
        { "(2*x) / (x*2)", "(/ (* 2 x) (* x 2))" },
        { "(2*x) ^ (x*2)", "(^ (* 2 x) (* x 2))" },

        { "2*1 + 2*1",     "(+ 2 2)" },
        { "2*1 - 2*1",     "(- 2 2)" },
        { "(2*1) * (2*1)", "(* 2 2)" },
        { "(2*1) / (2*1)", "(/ 2 2)" },
        { "(2*1) ^ (2*1)", "(^ 2 2)" },
    };
    for (const auto &test : s_test_data) {
        SECTION(test.input) {
            INFO("For input: " << test.input);
            REQUIRE(test_simplify(test.input) == test.expected);
        }
    }
}

TEST_CASE("Derive", "[derive][valid]") {
    static const TestDeriveData s_test_data[] = {
        // input, variable, expected
        { "42",   "x",       "0" },
        { "x",    "x",       "1" },
        { "x",    "y",       "0" },
        { "abra", "cadabra", "0" },

        { "x+1",    "x", "(+ 1 0)" },
        { "x-1",    "x", "(- 1 0)" },
        { "x*2",    "x", "(+ (* 1 2) (* x 0))" },
        { "x/2",    "x", "(/ (- (* 1 2) (* x 0)) (^ 2 2))" },
        { "x^2",    "x", "(* (^ x 2) (+ (/ (* 1 2) x) (* (log x) 0)))" },
        { "sin(x)", "x", "(* (cos x) 1)" },
        { "cos(x)", "x", "(* (- 0 (sin x)) 1)" },
        { "log(x)", "x", "(/ 1 x)" },
    };

    for (const auto &test : s_test_data) {
        SECTION(test.input) {
            INFO("For input: " << test.input);
            REQUIRE(test_derive(test.input, test.variable) == test.expected);
        }
    }
}

TEST_CASE("Derive and simplify", "[simplify]") {
    REQUIRE(test_derive_simplify("log(sin(2*x))", "x") == "(/ (* (cos (* 2 x)) 2) (sin (* 2 x)))");
}

TEST_CASE("Evaluate", "[evaluate][valid]") {
    static const TestEvaluateData s_test_data[] = {
        { "x+x*var",    {{"x", 3}, {"var", 13}}, 3+3*13 },
        { "x+x*var",    {{"x", 2}, {"var", 9}, {"asdasd", 42}}, 2+2*9 },

        { "42",         {},          42 },
        { "x",          {{"x", 38}}, 38 },
        { "sin(x)",     {{"x", 0}},   0 },
        { "cos(x)",     {{"x", 0}},   1 },
        { "log(x)",     {{"x", 1}},   0 },
        { "1+2",        {},           3 },
        { "1-2",        {},          -1 },
        { "2*3",        {},           6 },
        { "3/2",        {},           1.5 },
        { "3^2",        {},           9 },
        { "25^(0-1/2)", {},           0.2 },
    };

    for (const auto &test : s_test_data) {
        SECTION(test.input) {
            INFO("For input: " << test.input);
            REQUIRE(test_evaluate(test.input, test.variables) == test.expected);
        }
    }
}

#if !defined(RUNNER)
TEST_CASE("Equals", "[equals]") {
    auto E = create_expression_tree;

    SECTION("Same type") {
        CHECK(E("42") == E("42"));
        CHECK(E("42") != E("88"));

        CHECK(E("qqq") == E("qqq"));
        CHECK(E("qqq") != E("x"));

        CHECK(E("sin(1)") == E("sin(1)"));
        CHECK(E("sin(1)") != E("sin(2)"));

        CHECK(E("cos(x)") == E("cos(x)"));
        CHECK(E("cos(x)") != E("cos(y)"));

        CHECK(E("log(x)") == E("log(x)"));
        CHECK(E("log(x)") != E("log(y)"));
    }

    SECTION("Different types") {
        expr exprs[] = {
            E("1"), E("x"), E("sin(1)"), E("cos(1)"), E("log(1)"),
            E("1+1"),
            E("1+2"),
            E("2+1"),

            E("1-1"),
            E("1-2"),
            E("2-1"),

            E("1*1"),
            E("1*2"),
            E("2*1"),

            E("1/1"),
            E("1/2"),
            E("2/1"),

            E("1^1"),
            E("1^2"),
            E("2^1"),
        };
        for (size_t i=0; i<ARRAY_SIZE(exprs); ++i) {
            CHECK(exprs[i] == exprs[i]);
            for (size_t j=i+1; j<ARRAY_SIZE(exprs); ++j) {
                CHECK(exprs[i] != exprs[j]);
            }
        }
    }
}
#endif // !RUNNER


TEST_CASE("Error handling", "[errors]") {
#ifdef RUNNER
    #define E parse_and_write
#else
    #define E create_expression_tree
#endif

    CHECK_THROWS_MAYBEAS(E("1+(1"), parse_error);
    CHECK_THROWS_MAYBEAS(E("1+(1+2)+3)*1"), parse_error);
    CHECK_THROWS_MAYBEAS(E("1+"), parse_error);
    CHECK_THROWS_MAYBEAS(E("+"), parse_error);
    CHECK_THROWS_MAYBEAS(E(")"), parse_error);
    CHECK_THROWS_MAYBEAS(E("("), parse_error);
    CHECK_THROWS_MAYBEAS(E("()"), parse_error);
    CHECK_THROWS_MAYBEAS(E("sin(1 2)"), parse_error);
    CHECK_THROWS_MAYBEAS(E("sin()"), parse_error);
    CHECK_THROWS_MAYBEAS(E("(1+1)asd"), parse_error);
    CHECK_THROWS_MAYBEAS(E("(1+1)#"), tokenize_error);

    CHECK_THROWS_MAYBEAS(test_evaluate("log(0-1)", {}), domain_exception);
    CHECK_THROWS_MAYBEAS(test_evaluate("log(x)", {{"x", 0}}), domain_exception);
    CHECK_THROWS_MAYBEAS(test_evaluate("asd", {}), unbound_variable_exception);
    CHECK_THROWS_MAYBEAS(test_evaluate("sin", {}), unbound_variable_exception);

    CHECK_THROWS_MAYBEAS(parse_and_write("unknownfunc(1)"), unknown_function_exception);
    CHECK_THROWS_MAYBEAS(parse_and_write("sin 0"), parse_error);
}

#ifdef RUNNER
TEST_CASE("Bonus: Different print formats", "[bonus_print]") {
    static const struct {
        const char *input;
        const char *prefix;
        const char *infix;
        const char *postfix;
    } s_test_data[] = {
        { "1",                "1",        "1",        "1" },
        { "variable",         "variable", "variable", "variable" },
        { "sin(x)",           "(sin x)",  "sin(x)",     "x sin" },
        { "cos(x)",           "(cos x)",  "cos(x)",     "x cos" },
        { "log(x)",           "(log x)",  "log(x)",     "x log" },
        { "1+1",              "(+ 1 1)",  "(1 + 1)",  "1 1 +" },
        { "1-1",              "(- 1 1)",  "(1 - 1)",  "1 1 -" },
        { "1*1",              "(* 1 1)",  "(1 * 1)",  "1 1 *" },
        { "1/1",              "(/ 1 1)",  "(1 / 1)",  "1 1 /" },
        { "1^1",              "(^ 1 1)",  "(1 ^ 1)",  "1 1 ^" },

        // complex
        { "1+2*3",            "(+ 1 (* 2 3))",       "(1 + (2 * 3))",       "1 2 3 * +" },
        { "1+2*3*4",          "(+ 1 (* (* 2 3) 4))", "(1 + ((2 * 3) * 4))", "1 2 3 * 4 * +" },
        { "1*2^3",            "(* 1 (^ 2 3))",       "(1 * (2 ^ 3))",       "1 2 3 ^ *" },
        { "1*2^3^4",          "(* 1 (^ 2 (^ 3 4)))", "(1 * (2 ^ (3 ^ 4)))", "1 2 3 4 ^ ^ *" },

        { "(1+2)*3*4",        "(* (* (+ 1 2) 3) 4)", "(((1 + 2) * 3) * 4)", "1 2 + 3 * 4 *" },
        { "(1*2)^3^4",        "(^ (* 1 2) (^ 3 4))", "((1 * 2) ^ (3 ^ 4))", "1 2 * 3 4 ^ ^" },

        { "2^sin(1+1)",       "(^ 2 (sin (+ 1 1)))", "(2 ^ sin((1 + 1)))", "2 1 1 + sin ^" },
    };
    for (const auto &test : s_test_data) {
        INFO("For input: " << test.input);
        CHECK(parse_and_write(test.input, Commands::Print::Format::Prefix) == test.prefix);
        CHECK(parse_and_write(test.input, Commands::Print::Format::Infix) == test.infix);
        CHECK(parse_and_write(test.input, Commands::Print::Format::Postfix) == test.postfix);
    }
}
#endif // RUNNER
