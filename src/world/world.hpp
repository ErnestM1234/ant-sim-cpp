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

  World(int init_colonies_count) {
    // create the shared grid
    sharedGrid = SharedGrid();

    // create the colony grids
    for (int i = 0; i < init_colonies_count; i++) {
      colonyGrids.push_back(ColonyGrid());
    }
  }
};

#endif // WORLD_HPP