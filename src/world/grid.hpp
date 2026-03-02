#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>

template <typename T> struct Grid {
  // constants
  constexpr static const int DEFAULT_WIDTH = 100;
  constexpr static const int DEFAULT_HEIGHT = 100;
  constexpr static const float CELL_WIDTH = 10.0f;
  constexpr static const float CELL_HEIGHT = 10.0f;

  // properties
  int width;
  int height;
  float cellWidth;
  float cellHeight;
  std::vector<T> cells;

  Grid(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT,
       float cellWidth = CELL_WIDTH, float cellHeight = CELL_HEIGHT)
      : width(width), height(height), cellWidth(cellWidth),
        cellHeight(cellHeight) {
    cells.resize(height * width);
  }

  T &get(int x, int y) { return cells[y * width + x]; }

  void set(int x, int y, T value) { cells[y * width + x] = value; }

  /**
   * Renders a cell with a given value and color
   */
  void renderCell(sf::RenderWindow &window, int x, int y, sf::Color color) {
    sf::RectangleShape cell(sf::Vector2f(cellWidth, cellHeight));
    cell.setPosition(sf::Vector2f(x * cellWidth - cellWidth / 2,
                                  y * cellHeight - cellHeight / 2));
    cell.setFillColor(color);
    window.draw(cell);
  }
};

#endif // GRID_HPP