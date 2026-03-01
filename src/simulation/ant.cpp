#include "ant.hpp"

// ===== TRANSITIONS ===== //

AntState Ant::transition() {
  // TODO: Implement
  return AntState::SEARCHING;
}

// ===== STATES ===== //

void Ant::searchForFood(float dt) {
  static std::normal_distribution<float> wobble(0.0f, 1.0f);
  float angle = atan2(velocity.y, velocity.x);
  angle += wobble(rng()) * sqrt(dt) * WOBBLE_ANGLE;
  velocity = {cos(angle) * SPEED, sin(angle) * SPEED};
}

void Ant::returnToColony(float dt) {
  // TODO: Implement
}