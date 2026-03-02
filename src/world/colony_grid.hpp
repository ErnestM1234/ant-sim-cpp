#include "../world/grid.hpp"
#include <SFML/Graphics.hpp>

#ifndef COLONY_GRID_HPP
#define COLONY_GRID_HPP

struct ColonyCell {
  float toHomeScore = 0.0f;
  float toFoodScore = 0.0f;
};

struct ColonyGrid {
  // constants
  constexpr static const float DECAY_RATE = 0.01f;

  // properties
  Grid<ColonyCell> grid;

  ColonyGrid(int width, int height) : grid(width, height) {}

  void update(float dt) {
    for (int y = 0; y < grid.height; y++) {
      for (int x = 0; x < grid.width; x++) {
        grid.get(x, y).toHomeScore =
            fmax(0.0f, grid.get(x, y).toHomeScore - DECAY_RATE * dt);
        grid.get(x, y).toFoodScore =
            fmax(0.0f, grid.get(x, y).toFoodScore - DECAY_RATE * dt);
      }
    }
  }

  /**
   * Renders the colony grid
   * TODO: switch to using sf::VertexArray for performance
   */
  void render(sf::RenderWindow &window) {
    for (int y = 0; y < grid.height; y++) {
      for (int x = 0; x < grid.width; x++) {
        // draw the cell (transparent proportionate to the score)
        ColonyCell cell = grid.get(x, y);
        if (cell.toHomeScore > 0) {
          sf::Color color = sf::Color::Blue;
          color.a = static_cast<uint8_t>(
              std::clamp(cell.toHomeScore, 0.0f, 1.0f) * 255);
          grid.renderCell(window, x, y, color);
        }
        if (cell.toFoodScore > 0) {
          sf::Color color = sf::Color::Red;
          color.a = static_cast<uint8_t>(
              std::clamp(cell.toFoodScore, 0.0f, 1.0f) * 255);
          grid.renderCell(window, x, y, color);
        }
      }
    }
  }
};

#endif // COLONY_GRID_HPP