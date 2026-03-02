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

  bool foodDetected = false;
  float foodAngle = 0.0f;

  bool wallDetected = false;
  float wallAngle = 0.0f;

  bool homeDetected = false;
  float homeAngle = 0.0f;
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
  constexpr static const float STARTING_PHEREMONE_SCORE = 1.0f;
  constexpr static const float PHEREMONE_DECAY_RATE = 0.01f;

  // sample constants
  constexpr static const float DEFAULT_DETECTION_RADIUS =
      50.0f; // in world units
  constexpr static const float DEFAULT_DETECTION_ANGLE = 45.0f * M_PI / 180.0f;
  constexpr static const int DEFAULT_NUM_ANGLE_SAMPLES = 5;
  constexpr static const int DEFAULT_NUM_SAMPLES_PER_ANGLE = 5;
  constexpr static const bool SHOW_SAMPLING_CONE = false;
  constexpr static const bool SHOW_SAMPLING_DOTS = false;

  // position
  sf::Vector2f position;
  sf::Vector2f velocity;

  // world
  World &world;
  int colony_id;

  // behavior
  AntState state = AntState::SEARCHING;
  bool has_food = false;
  float pheromone_score = STARTING_PHEREMONE_SCORE;

  // colony
  sf::Vector2f colony_position;
  float colony_radius;
  float colony_radius_squared;

  // rendering
  sf::Vector2f radiusVector = {RADIUS, RADIUS};

  Ant(World &world, sf::Vector2f position, int colony_id,
      sf::Vector2f colony_position, float colony_radius)
      : world(world), position(position), colony_id(colony_id),
        colony_position(colony_position), colony_radius(colony_radius) {
    static std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f * M_PI);
    float angle = angle_dist(rng());
    velocity = {cos(angle) * SPEED, sin(angle) * SPEED};
    colony_radius_squared = colony_radius * colony_radius;
  }

  void update(float dt) {
    // decrement pheromone score
    decrementPheromoneScore(dt);

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
    if (SHOW_SAMPLING_CONE) {
      renderSamplingCone(window);
    }
  }

  /**
   * Helper function to render the sampling cone
   */
  void
  renderSamplingCone(sf::RenderWindow &window,
                     float detection_radius = DEFAULT_DETECTION_RADIUS,
                     float detection_angle = DEFAULT_DETECTION_ANGLE,
                     int num_angle_samples = DEFAULT_NUM_ANGLE_SAMPLES,
                     int num_samples_per_angle = DEFAULT_NUM_SAMPLES_PER_ANGLE);

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
   * Decrement the pheromone score
   */
  void decrementPheromoneScore(float dt) {
    pheromone_score = fmax(0.0f, pheromone_score - PHEREMONE_DECAY_RATE * dt);
  }

  /**
   * Samples the world in a cone shape
   * @param detection_radius The radius of the cone
   * @param detection_angle The angle of the cone (on either side)
   * @param num_angle_samples The number of angles to sample
   * @param num_samples_per_angle The number of samples to take per angle
   * @return The samples
   *
   * Right now, goes for strongest
   * TODO: heuristic accounting for distance too
   * TODO: GPU optimization, should look into feasibility/benefits
   */
  Sample sampleCone(float detection_radius = DEFAULT_DETECTION_RADIUS,
                    float detection_angle = DEFAULT_DETECTION_ANGLE,
                    int num_angle_samples = DEFAULT_NUM_ANGLE_SAMPLES,
                    int num_samples_per_angle = DEFAULT_NUM_SAMPLES_PER_ANGLE);
};

#endif // ANT_HPP