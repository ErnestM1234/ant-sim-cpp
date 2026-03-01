#include <SFML/Graphics.hpp>
#include "simulation/colony.hpp"
#include "render/renderer.hpp"
#include "simulation/simulator.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Ant Simulator");
    window.setFramerateLimit(60);

    sf::Clock clock;

    std::vector<Colony> colonies = { Colony({100, 100})};
    Renderer renderer(colonies);
    Simulator simulator(colonies);

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
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
