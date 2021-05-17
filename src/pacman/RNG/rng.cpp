#include "rng.hpp"

RNG::RNG() { range = std::mt19937(device()); }

RNG &RNG::operator=(const RNG &_rng) {
    range = _rng.range;
    distribution = _rng.distribution;
    return *this;
}

int RNG::get(const int min, const int max) {
    distribution = std::uniform_int_distribution<std::mt19937::result_type>(min, max);
    return distribution(range);
}