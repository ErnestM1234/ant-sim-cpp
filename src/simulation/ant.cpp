#include "ant.hpp"

#include "../world/colony_grid.hpp"

// ===== STATES ===== //

void Ant::searchForFood(float dt) {
  // handle pheromone trail
  ColonyCell &colonyCell =
      world.colonyGrids[colony_id].get(position.x, position.y);
  colonyCell.toFoodScore =
      fmin(1.0f, colonyCell.toFoodScore + TO_FOOD_SCORE_DEPOSIT);

  // handle remove food
  bool found_food = world.sharedGrid.get(position.x, position.y).foodScore > 0;
  if (found_food) {
    world.sharedGrid.get(position.x, position.y).foodScore = 0.0f;

    // update state
    this->has_food = true;
    this->state = AntState::RETURNING;
    return;
  }

  // follow go home pheromone trails
  Sample sample = sampleCone();
  if (sample.toHomeScore > 0) {
    velocity = {cos(sample.toHomeAngle) * SPEED,
                sin(sample.toHomeAngle) * SPEED};
    return;
  }

  // random wobble updates
  static std::normal_distribution<float> wobble(0.0f, 1.0f);
  float angle = atan2(velocity.y, velocity.x);
  angle += wobble(rng()) * sqrt(dt) * WOBBLE_ANGLE;
  velocity = {cos(angle) * SPEED, sin(angle) * SPEED};
}

void Ant::returnToColony(float dt) {
  // handle pheromone trail
  ColonyCell &colonyCell =
      world.colonyGrids[colony_id].get(position.x, position.y);
  colonyCell.toHomeScore =
      fmin(1.0f, colonyCell.toHomeScore + TO_HOME_SCORE_DEPOSIT);

  // random wobble updates
  static std::normal_distribution<float> wobble(0.0f, 1.0f);
  float angle = atan2(velocity.y, velocity.x);
  angle += wobble(rng()) * sqrt(dt) * WOBBLE_ANGLE;
  velocity = {cos(angle) * SPEED, sin(angle) * SPEED};
}

Sample Ant::sampleCone(float detection_radius, float detection_angle,
                       int num_angle_samples, int num_samples_per_angle) {
  float heading = atan2(velocity.y, velocity.x);
  Sample best{.toFoodScore = 0.0f,
              .toFoodAngle = 0.0f,
              .toHomeScore = 0.0f,
              .toHomeAngle = 0.0f,
              .foodScore = 0.0f,
              .foodAngle = 0.0f,
              .wallDetected = false,
              .wallAngle = 0.0f};
  float best_wall_distance = std::numeric_limits<float>::max();

  // sample the world in a cone shape
  for (int i = 0; i < num_angle_samples; i++) {
    for (int j = 1; j <= num_samples_per_angle; j++) {
      // Get location
      float angle = heading - detection_angle +
                    (2.0f * detection_angle * i) / (num_angle_samples - 1);
      sf::Vector2f sample_pos = world.sharedGrid.getToroidalPosition(
          position + sf::Vector2f{cos(angle), sin(angle)} *
                         (detection_radius * j / num_samples_per_angle));

      // Get intensity score
      float toHomeScore =
          world.colonyGrids[colony_id].get(sample_pos).toHomeScore;
      if (toHomeScore > best.toHomeScore) {
        best.toHomeScore = toHomeScore;
        best.toHomeAngle = angle;
      }
      float toFoodScore =
          world.colonyGrids[colony_id].get(sample_pos).toFoodScore;
      if (toFoodScore > best.toFoodScore) {
        best.toFoodScore = toFoodScore;
        best.toFoodAngle = angle;
      }
      float foodScore = world.sharedGrid.get(sample_pos).foodScore;
      if (foodScore > best.foodScore) {
        best.foodScore = foodScore;
        best.foodAngle = angle;
      }
      // closest wall
      float wall_score = world.sharedGrid.get(sample_pos).wallScore;
      if (wall_score > 0) {
        float wall_distance = (sample_pos - position).lengthSquared();
        if (wall_distance < best_wall_distance) {
          best_wall_distance = wall_distance;
          best.wallDetected = true;
          best.wallAngle = angle;
        }
      }
    }
  }

  return best;
}
