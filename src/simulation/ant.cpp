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
    pheromone_score = STARTING_PHEREMONE_SCORE;

    // update state
    this->has_food = true;
    this->state = AntState::RETURNING;
    return;
  }

  // follow go home pheromone trails
  Sample sample = sampleCone();
  if (sample.foodDetected) {
    // follow food trail
    velocity = {cos(sample.foodAngle) * SPEED, sin(sample.foodAngle) * SPEED};
    return;
  } else if (sample.toHomeScore > 0) {
    // follow home pheromone trail
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

  // handle meet colony
  float distance_to_colony_squared =
      (position - colony_position).lengthSquared();
  if (distance_to_colony_squared < colony_radius_squared) {
    this->has_food = false;
    this->state = AntState::SEARCHING;
    pheromone_score = STARTING_PHEREMONE_SCORE;
    return;
  }

  // follow trails
  Sample sample = sampleCone();
  if (sample.homeDetected) {
    // follow home pheromone trail
    velocity = {cos(sample.homeAngle) * SPEED, sin(sample.homeAngle) * SPEED};
    return;
  } else if (sample.toFoodScore > 0) {
    // follow food trail
    velocity = {cos(sample.toFoodAngle) * SPEED,
                sin(sample.toFoodAngle) * SPEED};
    return;
  }

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
              .foodDetected = false,
              .foodAngle = 0.0f,
              .wallDetected = false,
              .wallAngle = 0.0f,
              .homeDetected = false,
              .homeAngle = 0.0f};
  float best_food_distance = std::numeric_limits<float>::max();
  float best_wall_distance = std::numeric_limits<float>::max();
  float best_home_distance = std::numeric_limits<float>::max();

  // sample the world in a cone shape
  for (int i = 0; i < num_angle_samples; i++) {
    // Get angle
    float angle = heading - detection_angle +
                  (2.0f * detection_angle * i) / (num_angle_samples - 1);
    for (int j = 1; j <= num_samples_per_angle; j++) {
      // Get location
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
      // closest food
      float foodScore = world.sharedGrid.get(sample_pos).foodScore;
      if (foodScore > 0) {
        float food_distance = (sample_pos - position).lengthSquared();
        if (food_distance < best_food_distance) {
          best_food_distance = foodScore;
          best.foodAngle = angle;
          best.foodDetected = true;
        }
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
      // distance to home
      float home_distance = (sample_pos - colony_position).lengthSquared();
      if (home_distance < colony_radius_squared) {
        best_home_distance = home_distance;
        best.homeDetected = true;
        best.homeAngle = angle;
      }
    }
  }

  return best;
}

void Ant::renderSamplingCone(sf::RenderWindow &window, float detection_radius,
                             float detection_angle, int num_angle_samples,
                             int num_samples_per_angle) {
  float heading = atan2(velocity.y, velocity.x);
  sf::Color color(255, 255, 0, 40); // faint yellow
  sf::VertexArray cone(sf::PrimitiveType::TriangleFan);
  sf::VertexArray dots(sf::PrimitiveType::Points);
  cone.append({position, color}); // center point

  for (int i = 0; i <= num_angle_samples; i++) {
    // Get angle
    float angle = heading - detection_angle +
                  (2.0f * detection_angle * i) / num_angle_samples;
    sf::Vector2f tip =
        position + sf::Vector2f{cos(angle), sin(angle)} * detection_radius;
    cone.append({tip, color});
    if (!SHOW_SAMPLING_DOTS) {
      continue;
    }
    for (int j = 1; j <= num_samples_per_angle; j++) {
      // Get location
      sf::Vector2f sample_pos = world.sharedGrid.getToroidalPosition(
          position + sf::Vector2f{cos(angle), sin(angle)} *
                         (detection_radius * j / num_samples_per_angle));
      dots.append({sample_pos, sf::Color::White});
    }
  }

  if (SHOW_SAMPLING_DOTS) {
    window.draw(dots);
  }

  window.draw(cone);
}