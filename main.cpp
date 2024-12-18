#include <SFML/Graphics.hpp>
#include "includes/bodies/BodyGenerator.h"
#include "includes/qtree/QuadTree.h"
#include "includes/qtree/BoundingBox.h"
#include "includes/renderer/Renderer.h"
#include "includes/universe/Universe.h"

#include <memory>
#include <iostream>

using namespace std;

int main() {

  srand(time(0));

  // Setup the window and renderer
  sf::RenderWindow window(sf::VideoMode(1000, 700), "N-Body");
  Renderer renderer(window);
  
  // Setup the universe
  Universe universe(20000, renderer);

  while (window.isOpen()) {

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    // Render the universe
    window.clear();   
    universe.evolve(0.1);
    window.display();
  }

  return 0;
}