#ifndef ANT_HPP
#define ANT_HPP

#include "../utils/random.hpp"
#include "../world/world.hpp"
#include <SFML/Graphics.hpp>

enum class AntState {
  SEARCHING, // searching for food
  RETURNING, // returning to the colony
};

/**
 * Represents a sample of the world
 */
struct Sample {
  float toFoodScore = 0.0f;
  float toFoodAngle = 0.0f;

  float toHomeScore = 0.0f;
  float toHomeAngle = 0.0f;

  float foodScore = 0.0f;
  float foodAngle = 0.0f;

  bool wallDetected = false;
  float wallAngle = 0.0f;
};

/**
 * Ants governed by simple state machine
 */
struct Ant {
  // constants
  constexpr static const float TO_FOOD_SCORE_DEPOSIT =
      1.0f; // how much to add to the "to food score"
  constexpr static const float TO_HOME_SCORE_DEPOSIT =
      1.0f; // how much to add to the "to home score"
  constexpr static const float WOBBLE_ANGLE = 90.0f * M_PI / 180.0f;
  constexpr static const float SPEED = 100.0f;
  constexpr static const float RADIUS = 2;

  // sample constants
  constexpr static const float DEFAULT_DETECTION_RADIUS =
      100.0f; // in world units
  constexpr static const float DEFAULT_DETECTION_ANGLE = 45.0f * M_PI / 180.0f;
  constexpr static const int DEFAULT_NUM_SAMPLES = 10;

  // position
  sf::Vector2f position;
  sf::Vector2f velocity;

  // world
  World &world;
  int colony_id;

  // behavior
  AntState state = AntState::SEARCHING;
  bool has_food = false;

  // rendering
  sf::Vector2f radiusVector = {RADIUS, RADIUS};

  Ant(World &world, sf::Vector2f position, int colony_id)
      : world(world), position(position), colony_id(colony_id) {
    static std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f * M_PI);
    float angle = angle_dist(rng());
    velocity = {cos(angle) * SPEED, sin(angle) * SPEED};
  }

  void update(float dt) {
    // execute behavior
    switch (state) {
    case AntState::SEARCHING:
      searchForFood(dt);
      break;
    case AntState::RETURNING:
      returnToColony(dt);
      break;
    }

    // update position (wrapping around the grid)
    position = world.sharedGrid.getToroidalPosition(position + velocity * dt);
  }

  void render(sf::RenderWindow &window) {
    sf::CircleShape shape(RADIUS);
    shape.setPosition(position - radiusVector);
    switch (state) {
    case AntState::SEARCHING:
      shape.setFillColor(sf::Color::Red);
      break;
    case AntState::RETURNING:
      shape.setFillColor(sf::Color::Blue);
      break;
    }
    window.draw(shape);
  }

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

  // ===== UTILITIES ===== //

  /**
   * Samples the world in a cone shape
   * @param detection_radius The radius of the cone
   * @param detection_angle The angle of the cone (on either side)
   * @param num_samples The number of samples to take
   * @return The samples
   *
   * Right now, goes for strongest
   * TODO: heuristic accounting for distance too
   * TODO: GPU optimization, should look into feasibility/benefits
   */
  Sample sampleCone(float detection_radius = DEFAULT_DETECTION_RADIUS,
                    float detection_angle = DEFAULT_DETECTION_ANGLE,
                    int num_samples = DEFAULT_NUM_SAMPLES);
};

#endif // ANT_HPP