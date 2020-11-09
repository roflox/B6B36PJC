#pragma once

#include <memory>
#include <limits>

enum class generator_kind {
    random,
    mersenne,
    minstd
};

class numbers_generator {
public:

    size_t doubles = 0;
    size_t bools = 0;
    size_t falses = 0;
    size_t ints = 0;
    int maxInt = std::numeric_limits<int>::min();
    int minInt = std::numeric_limits<int>::max();
    double minDouble = std::numeric_limits<double>::max();
    double maxDouble = std::numeric_limits<double>::min();

    /**
     * Vygeneruje náhodné číslo v intervalu [0, 1)
     */
    virtual double random_double() = 0;

    /**
     * Vygeneruje náhodné celé číslo v intervalu [lower, upper]
     */
    virtual int random_int(int lower, int upper) = 0;

    /**
     * Vygeneruje náhodnou pravdivostní hodnotu (true/false)
     */
    virtual bool random_bool() = 0;

    virtual ~numbers_generator() = default;

    static std::unique_ptr<numbers_generator> create_by_kind(generator_kind kind, size_t seed);

    double min_generated_double() { return minDouble; }

    double max_generated_double() { return maxDouble; }

    size_t doubles_generated() { return doubles; }

    int min_generated_int() { return minInt; }

    int max_generated_int() { return maxInt; }

    size_t ints_generated() { return ints; }

    size_t trues_generated() { return bools - falses; }

    size_t falses_generated() { return falses; }

    size_t bools_generated() { return bools; }
};
