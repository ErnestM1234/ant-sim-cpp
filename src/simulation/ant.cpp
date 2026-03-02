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
  bool has_food = world.sharedGrid.get(position.x, position.y).foodScore > 0;
  if (has_food) {
    world.sharedGrid.get(position.x, position.y).foodScore = 0.0f;

    // update state
    has_food = true;
    state = AntState::RETURNING;
    return;
  }

  // handle movement updates
  static std::normal_distribution<float> wobble(0.0f, 1.0f);
  float angle = atan2(velocity.y, velocity.x);
  angle += wobble(rng()) * sqrt(dt) * WOBBLE_ANGLE;
  velocity = {cos(angle) * SPEED, sin(angle) * SPEED};
}

void Ant::returnToColony(float dt) {
  // TODO: Implement
}