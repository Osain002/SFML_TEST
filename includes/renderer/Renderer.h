#pragma once
#include "../qtree/QuadTree.h"
#include <SFML/Graphics.hpp>

class Renderer {

  public:

    Renderer(sf::RenderWindow& view): window(view) {}

    // Draw a rectangle
    void draw_rectangle(double x, double y, double width, double height);

    // Draw a point
    void draw_point(double x, double y);
    
    // Draw a quadtree
    void draw_tree(QuadTree& tree);

    // Draw a bunch of points
    void draw_points(std::shared_ptr<vector<BodyGenerator::Body>>& points);
  
  private: 
    sf::RenderWindow& window;

};