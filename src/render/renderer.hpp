#include <SFML/Graphics.hpp>
#include "simulation/colony.hpp"

#ifndef RENDERER_HPP
#define RENDERER_HPP

/**
 * Handles the rendering of the colonies and ants
 **/
struct Renderer
{
  std::vector<Colony>& colonies;

  Renderer(std::vector<Colony>& colonies) : colonies(colonies) {}

  void render(sf::RenderWindow& window) {
    for (auto& colony : colonies) {
      colony.render(window);
      for (auto& ant : colony.ants) {
        ant.render(window);
      }
    }
  }
};

#endif // RENDERER_HPP