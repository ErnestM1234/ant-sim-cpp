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

    // Temp: inject food into the shared grid
    int food_x = 800;
    int food_y = 0;
    for (int i = 0; i < 50; i++) {
      for (int j = 0; j < 50; j++) {
        sharedGrid.getOnGrid(food_x + i, food_y + j).foodScore = 1.0f;
      }
    }

    // create the colony grids
    for (int i = 0; i < init_colonies_count; i++) {
      colonyGrids.push_back(ColonyGrid());
    }
  }

  void update(float dt) {
    sharedGrid.update(dt);
    for (auto &colonyGrid : colonyGrids) {
      colonyGrid.update(dt);
    }
  }

  void render(sf::RenderWindow &window) {
    sharedGrid.render(window);
    for (auto &colonyGrid : colonyGrids) {
      colonyGrid.render(window);
    }
  }
};

#endif // WORLD_HPP