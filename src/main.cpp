#include "render/renderer.hpp"
#include "simulation/colony.hpp"
#include "simulation/simulator.hpp"
#include "world/world.hpp"
#include <SFML/Graphics.hpp>

const int COLONY_COUNT = 1;

int main() {
  sf::RenderWindow window(sf::VideoMode({1200, 800}), "Ant Simulator");
  window.setFramerateLimit(60);

  sf::Clock clock;

  World world = World(COLONY_COUNT);
  std::vector<Colony> colonies = {};
  for (int i = 0; i < COLONY_COUNT; i++) {
    colonies.push_back(Colony(world, {100.0f + i * 100.0f, 100.0f}, i));
  }
  Renderer renderer(colonies);
  Simulator simulator(colonies);

  while (window.isOpen()) {
    while (const auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();

      if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Escape)
          window.close();
      }
    }

    float dt = clock.restart().asSeconds();
    (void)dt; // will be used for simulation updates

    window.clear(sf::Color(30, 30, 30));

    // update items
    simulator.update(dt);

    // render items
    renderer.render(window);

    window.display();
  }

  return 0;
}
