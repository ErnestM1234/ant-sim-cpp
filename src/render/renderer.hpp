#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "simulation/colony.hpp"
#include <SFML/Graphics.hpp>

/**
 * Handles the rendering of the colonies and ants
 **/
struct Renderer {
  std::vector<Colony> &colonies;
  World &world;

  Renderer(std::vector<Colony> &colonies, World &world)
      : colonies(colonies), world(world) {}

  void render(sf::RenderWindow &window) {
    for (auto &colony : colonies) {
      colony.render(window);
      for (auto &ant : colony.ants) {
        ant.render(window);
      }
    }
    world.sharedGrid.render(window);
    for (auto &colonyGrid : world.colonyGrids) {
      colonyGrid.render(window);
    }
  }
};

#endif // RENDERER_HPP