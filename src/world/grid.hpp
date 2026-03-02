#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "../config.hpp"
template <typename T> struct Grid {
  // constants
  constexpr static const float DEFAULT_X_OFFSET = 0.0f;
  constexpr static const float DEFAULT_Y_OFFSET = 0.0f;
  constexpr static const float DEFAULT_CELL_WIDTH = 1.0f;
  constexpr static const float DEFAULT_CELL_HEIGHT = 1.0f;
  constexpr static const int DEFAULT_WIDTH =
      Config::WINDOW_WIDTH / DEFAULT_CELL_WIDTH;
  constexpr static const int DEFAULT_HEIGHT =
      Config::WINDOW_HEIGHT / DEFAULT_CELL_HEIGHT;

  // properties
  int width;
  int height;
  float cellWidth;
  float cellHeight;
  float xOffset;
  float yOffset;
  std::vector<T> cells;

  Grid(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT,
       float cellWidth = DEFAULT_CELL_WIDTH,
       float cellHeight = DEFAULT_CELL_HEIGHT, float xOffset = DEFAULT_X_OFFSET,
       float yOffset = DEFAULT_Y_OFFSET)
      : width(width), height(height), cellWidth(cellWidth),
        cellHeight(cellHeight), xOffset(xOffset), yOffset(yOffset) {
    cells.resize(height * width);
  }

  // ===== GETTERS AND SETTERS ===== //

  // Get via grid coordinates
  T &get(int x, int y) { return cells[y * width + x]; }
  // Get via position coordinates
  T &get(float x, float y) {
    return get(static_cast<int>((x - xOffset) / cellWidth),
               static_cast<int>((y - yOffset) / cellHeight));
  }
  T &get(sf::Vector2f position) { return get(position.x, position.y); }

  // Set via grid coordinates
  void set(int x, int y, T value) { cells[y * width + x] = value; }
  // Set via position coordinates
  void set(float x, float y, T value) {
    set(static_cast<int>((x - xOffset) / cellWidth),
        static_cast<int>((y - yOffset) / cellHeight), value);
  }
  void set(sf::Vector2f position, T value) {
    set(position.x, position.y, value);
  }

  // ===== RENDERING ===== //

  /**
   * Renders a cell with a given value and color
   */
  void renderCell(sf::RenderWindow &window, int x, int y, sf::Color color) {
    sf::RectangleShape cell(sf::Vector2f(cellWidth, cellHeight));
    cell.setPosition(
        sf::Vector2f((x * cellWidth + xOffset), (y * cellHeight + yOffset)));
    cell.setFillColor(color);
    window.draw(cell);
  }
};

#endif // GRID_HPP