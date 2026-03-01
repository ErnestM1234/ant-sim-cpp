#include <SFML/Graphics.hpp>
#include "ant.hpp"

/**
 * Represents the base colony for ants
 **/

#ifndef COLONY_HPP
#define COLONY_HPP

struct Colony
{
  // constants
  constexpr static const float RADIUS = 100;
  constexpr static const int DEFAULT_NUM_ANTS = 100;

  // position
  sf::Vector2f position;
  std::vector<Ant> ants;

  // rendering
  sf::Vector2f radiusVector = {RADIUS, RADIUS};

  Colony(sf::Vector2f position, int numAnts = DEFAULT_NUM_ANTS): position(position) {
    for (int i = 0; i < numAnts; i++) {
      ants.push_back(Ant(position));
    }
  }

  void update(float dt) {
    for (auto& ant : ants) {
      ant.update(dt);
    }
  }

  void render(sf::RenderWindow& window) {
    // draw colony
    sf::CircleShape shape(RADIUS);
    shape.setPosition(position - radiusVector);
    shape.setFillColor(sf::Color::Blue);
    window.draw(shape);
  }
};

#endif // COLONY_HPP