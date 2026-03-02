#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "colony.hpp"
#include "world/world.hpp"
#include <SFML/Graphics.hpp>

/**
 * Handles the updating of the colonies and ants
 **/
struct Simulator {
  std::vector<Colony> &colonies;
  World &world;

  Simulator(std::vector<Colony> &colonies, World &world)
      : colonies(colonies), world(world) {}

  void update(float dt) {
    for (auto &colony : colonies) {
      colony.update(dt);
    }
    world.update(dt);
  }
};

#endif // SIMULATOR_HPP