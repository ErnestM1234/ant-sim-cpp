#ifndef SHARED_GRID_HPP
#define SHARED_GRID_HPP

#include "../world/grid.hpp"

/**
 * Technically these are mutally exclusive but keeping as float for future
 * expansion
 **/
struct SharedCell {
  float wallScore = 0.0f;
  float foodScore = 0.0f;
};

struct SharedGrid : Grid<SharedCell> {

  SharedGrid() : Grid<SharedCell>() {}
  SharedGrid(int width, int height) : Grid<SharedCell>(width, height) {}

  void update(float dt) {
    // No-op
  }

  void render(sf::RenderWindow &window) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        SharedCell cell = get(x, y);
        if (cell.wallScore > 0) {
          sf::Color color = sf::Color::White;
          renderCell(window, x, y, color);
        } else if (cell.foodScore > 0) {
          sf::Color color = sf::Color::Yellow;
          renderCell(window, x, y, color);
        }
      }
    }
  }
};

#endif // SHARED_GRID_HPP