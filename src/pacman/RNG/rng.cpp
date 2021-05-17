#include "rng.hpp"

RNG::RNG(const int min, const int max) {
    range = std::mt19937(device());
    distribution = std::uniform_int_distribution<std::mt19937::result_type>(min, max);
}

RNG &RNG::operator=(const RNG &_rng) {
    range = _rng.range;
    distribution = _rng.distribution;
    return *this;
}

int RNG::get() { return distribution(range); }