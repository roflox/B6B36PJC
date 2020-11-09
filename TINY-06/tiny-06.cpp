#include "tiny-06.hpp"

#include <random>
#include <cstdint>

// Let's hope your std::random_device is implemented well.
// !! don't use as actual CSPRNG !!
class random_generator : public numbers_generator {
public:
    random_generator() = default;

    double random_double() override {
        double dab = double_dist(rdev);
        this->doubles++;
        if (dab < this->minDouble) {
            this->minDouble = dab;
        }
        if (dab > this->maxDouble) {
            this->maxDouble = dab;
        }
        return dab;
    }

    int random_int(int lower, int upper) override {
        int i = std::uniform_int_distribution<int>(lower, upper)(rdev);
        this->ints++;
        if (i < this->minInt) {
            this->minInt = i;
        }
        if (i > this->maxInt) {
            this->maxInt = i;
        }
        return i;
    }

    bool random_bool() override {
        auto bul = bool_dist(rdev);
        if (!bul) {
            this->falses++;
        }
        this->bools++;
        return bul;
    }

    ~random_generator() override = default;

private:
    std::uniform_real_distribution<double> double_dist{0, 1};
    std::bernoulli_distribution bool_dist{0.5};
    std::random_device rdev;
};

class mersenne_twister_generator : public numbers_generator {
public:
    mersenne_twister_generator(size_t seed) :
            rng(seed) {}

    double random_double() override {
        double dab = double_dist(rng);
        this->doubles++;
        if (dab < this->minDouble) {
            this->minDouble = dab;
        }
        if (dab > this->maxDouble) {
            this->maxDouble = dab;
        }
        return dab;
    }

    int random_int(int lower, int upper) override {
        int i = std::uniform_int_distribution<int>(lower, upper)(rng);
        this->ints++;
        if (i < this->minInt) {
            this->minInt = i;
        }
        if (i > this->maxInt) {
            this->maxInt = i;
        }
        return i;
    }

    bool random_bool() override {
        auto bul = bool_dist(rng);
        if (!bul) {
            this->falses++;
        }
        this->bools++;
        return bul;
    }

    ~mersenne_twister_generator() override = default;

private:
    std::uniform_real_distribution<double> double_dist{0, 1};
    std::bernoulli_distribution bool_dist{0.5};
    std::mt19937_64 rng;
};

class minstd_generator : public numbers_generator {
public:
    minstd_generator(size_t seed) :
            rng(static_cast<uint32_t>(seed)) {}

    double random_double() override {
        double dab = double_dist(rng);
        this->doubles++;
        if (dab < this->minDouble) {
            this->minDouble = dab;
        }
        if (dab > this->maxDouble) {
            this->maxDouble = dab;
        }
        return dab;
    }

    int random_int(int lower, int upper) override {
        int i = std::uniform_int_distribution<int>(lower, upper)(rng);
        this->ints++;
        if (i < this->minInt) {
            this->minInt = i;
        }
        if (i > this->maxInt) {
            this->maxInt = i;
        }
        return i;
    }

    bool random_bool() override {
        auto bul = bool_dist(rng);
        if (!bul) {
            this->falses++;
        }
        this->bools++;
        return bul;
    }

private:
    std::uniform_real_distribution<double> double_dist{0, 1};
    std::bernoulli_distribution bool_dist{0.5};
    std::minstd_rand rng;
};

std::unique_ptr<numbers_generator> numbers_generator::create_by_kind(generator_kind kind, size_t seed) {
    switch (kind) {
        case generator_kind::random:
            return std::make_unique<random_generator>();
        case generator_kind::mersenne:
            return std::make_unique<mersenne_twister_generator>(seed);
        case generator_kind::minstd:
            return std::make_unique<minstd_generator>(seed);
        default:
            throw std::invalid_argument("Asked for unknown kind of generator!");
    }
}