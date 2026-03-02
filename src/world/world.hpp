#ifndef WORLD_HPP
#define WORLD_HPP

#include "colony_grid.hpp"
#include "shared_grid.hpp"

/**
 * Grid cell management
 **/
struct World {
  // properties
  SharedGrid sharedGrid;
  std::vector<ColonyGrid> colonyGrids;
};

#endif // WORLD_HPP