#ifndef COLONY_GRID_HPP
#define COLONY_GRID_HPP

#include "../world/grid.hpp"
#include <SFML/Graphics.hpp>

struct ColonyCell {
  float toHomeScore = 0.0f;
  float toFoodScore = 0.0f;
};

struct ColonyGrid : Grid<ColonyCell> {
  // constants
  constexpr static const float DECAY_RATE = 0.01f;

  // properties
  ColonyGrid() : Grid<ColonyCell>() {}
  ColonyGrid(int width, int height) : Grid<ColonyCell>(width, height) {}

  void update(float dt) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        get(x, y).toHomeScore =
            fmax(0.0f, get(x, y).toHomeScore - DECAY_RATE * dt);
        get(x, y).toFoodScore =
            fmax(0.0f, get(x, y).toFoodScore - DECAY_RATE * dt);
      }
    }
  }

  /**
   * Renders the colony grid
   * TODO: switch to using sf::VertexArray for performance
   */
  void render(sf::RenderWindow &window) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        // draw the cell (transparent proportionate to the score)
        ColonyCell cell = get(x, y);
        if (cell.toHomeScore > 0) {
          sf::Color color = sf::Color::Blue;
          color.a = static_cast<uint8_t>(
              std::clamp(cell.toHomeScore, 0.0f, 1.0f) * 255);
          renderCell(window, x, y, color);
        }
        if (cell.toFoodScore > 0) {
          sf::Color color = sf::Color::Red;
          color.a = static_cast<uint8_t>(
              std::clamp(cell.toFoodScore, 0.0f, 1.0f) * 255);
          renderCell(window, x, y, color);
        }
      }
    }
  }
};

#endif // COLONY_GRID_HPP