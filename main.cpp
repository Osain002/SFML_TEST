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
  sf::RenderWindow window(sf::VideoMode(500, 500), "N-Body");
  Renderer renderer(window);
  
  // Setup the universe
  Universe universe(30000, renderer);

  while (window.isOpen()) {

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    // Render the universe
    window.clear();   
    universe.evolve(0.01);
    window.display();
  }

  return 0;
}