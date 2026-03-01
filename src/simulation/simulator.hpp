#include <SFML/Graphics.hpp>
#include "colony.hpp"

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

/**
 * Handles the updating of the colonies and ants
 **/
struct Simulator
{
  std::vector<Colony>& colonies;

  Simulator(std::vector<Colony>& colonies) : colonies(colonies) {}

  void update(float dt) {
    for (auto& colony : colonies) {
      colony.update(dt);
    }
  }
};

#endif // SIMULATOR_HPP