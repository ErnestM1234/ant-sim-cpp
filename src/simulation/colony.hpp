#ifndef COLONY_HPP
#define COLONY_HPP

#include "../world/world.hpp"
#include "ant.hpp"
#include <SFML/Graphics.hpp>

/**
 * Represents the base colony for ants
 **/

struct Colony {
  // constants
  constexpr static const float RADIUS = 100;
  constexpr static const int DEFAULT_NUM_ANTS = 100;

  // position
  sf::Vector2f position;
  std::vector<Ant> ants;

  // rendering
  sf::Vector2f radiusVector = {RADIUS, RADIUS};

  // other
  int colony_id;

  Colony(World &world, sf::Vector2f position, int colony_id,
         int numAnts = DEFAULT_NUM_ANTS)
      : position(position), colony_id(colony_id) {
    for (int i = 0; i < numAnts; i++) {
      ants.push_back(Ant(world, position, colony_id, position, RADIUS));
    }
  }

  void update(float dt) {
    for (auto &ant : ants) {
      ant.update(dt);
    }
  }

  void render(sf::RenderWindow &window) {
    // draw colony
    sf::CircleShape shape(RADIUS);
    shape.setPosition(position - radiusVector);
    shape.setFillColor(sf::Color::Blue);
    window.draw(shape);
  }
};

#endif // COLONY_HPP