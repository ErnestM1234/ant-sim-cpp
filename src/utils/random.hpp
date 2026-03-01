#include <random>

#ifndef RANDOM_HPP
#define RANDOM_HPP

/**
 * Returns a reference to a random number generator
 */
 inline std::mt19937& rng() {
  static std::mt19937 gen(std::random_device{}());
  return gen;
}

#endif // RANDOM_HPP