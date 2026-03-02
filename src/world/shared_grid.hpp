#include "../world/grid.hpp"

#ifndef SHARED_GRID_HPP
#define SHARED_GRID_HPP

/**
 * Technically these are mutally exclusive but keeping as float for future
 * expansion
 **/
struct SharedCell {
  float wallScore = 0.0f;
  float foodScore = 0.0f;
};

struct SharedGrid {

  // properties
  Grid<SharedCell> grid;

  SharedGrid(int width, int height) : grid(width, height) {}

  void update(float dt) {
    // No-op
  }

  void render(sf::RenderWindow &window) {
    for (int y = 0; y < grid.height; y++) {
      for (int x = 0; x < grid.width; x++) {
        SharedCell cell = grid.get(x, y);
        if (cell.wallScore > 0) {
          sf::Color color = sf::Color::White;
          grid.renderCell(window, x, y, color);
        } else if (cell.foodScore > 0) {
          sf::Color color = sf::Color::Yellow;
          grid.renderCell(window, x, y, color);
        }
      }
    }
  }
};

#endif // SHARED_GRID_HPP