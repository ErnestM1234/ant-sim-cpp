#ifndef ANT_HPP
#define ANT_HPP

#include <SFML/Graphics.hpp>
#include "../utils/random.hpp"


enum class AntState {
  SEARCHING, // searching for food
  RETURNING, // returning to the colony
};

/**
 * Ants governed by simple state machine
 */
struct Ant
{
  // constants
  constexpr static const float WOBBLE_ANGLE = 90.0f * M_PI / 180.0f;
  constexpr static const float SPEED = 100.0f;
  constexpr static const float RADIUS = 2;

  // position
  sf::Vector2f position;
  sf::Vector2f velocity;

  // behavior
  AntState state = AntState::SEARCHING;

  // rendering
  sf::Vector2f radiusVector = {RADIUS, RADIUS};

  Ant(sf::Vector2f position) : position(position) {
    static std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f *
      M_PI);
    float angle = angle_dist(rng());
    velocity = {cos(angle) * SPEED, sin(angle) * SPEED};
  }

  void update(float dt) {
    // update state
    state = transition();

    // execute behavior
    switch (state) {
      case AntState::SEARCHING:
        searchForFood(dt);
        break;
      case AntState::RETURNING:
        returnToColony(dt);
        break;
    }

    // update position
    position += velocity * dt;
  }

  void render(sf::RenderWindow& window) {
    sf::CircleShape shape(RADIUS);
    shape.setPosition(position - radiusVector);
    shape.setFillColor(sf::Color::Red);
    window.draw(shape);
  }

  // ===== TRANSITIONS ===== //

  AntState transition();

  // ===== STATES ===== //
  // each type of STATE is a different behavior

  /**
   * Searches for food - explores in random directions
   * Add +/- WOBBLE_ANGLE degrees to the current angle per time step
   */
  void searchForFood(float dt);

  /**
   * Returns to the colony
   */
  void returnToColony(float dt);


};

#endif // ANT_HPP