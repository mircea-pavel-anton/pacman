#ifndef RNG_HPP
#define RNG_HPP

#include <random>

typedef struct RNG {
  public:
    RNG() : RNG(0, 100) {};
    RNG(const int min, const int max);

    int get();

    RNG &operator=(const RNG &_rng);

  private:
    std::random_device device;
    std::mt19937 range;
    std::uniform_int_distribution<std::mt19937::result_type> distribution;

} RNG;

#endif