#ifndef RNG_HPP
#define RNG_HPP

#include <random>

typedef struct RNG {
  public:
    RNG();
    int get(const int, const int);

    RNG &operator=(const RNG &_rng);

  private:
    std::random_device device;
    std::mt19937 range;
    std::uniform_int_distribution<std::mt19937::result_type> distribution;

} RNG;

#endif