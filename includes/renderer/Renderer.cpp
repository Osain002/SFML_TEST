#include <iostream>
#include "Renderer.h"
#include "../qtree/BoundingBox.h"

// Draw a rectangle
void Renderer::draw_rectangle(double x, double y, double width, double height) {
  sf::RectangleShape rectangle(sf::Vector2f(width, height));
  rectangle.setOutlineColor(sf::Color::Red);
  rectangle.setOutlineThickness(0.5f);
  rectangle.setFillColor(sf::Color::Transparent);
  rectangle.setPosition(x, y);
  window.draw(rectangle);
}

// Draw a point
void Renderer::draw_point(double x, double y) {
  sf::CircleShape shape(0.5f);
  shape.setFillColor(sf::Color::Green);
  shape.setPosition(x, y);
  window.draw(shape);
}

// Draw a quadtree
void Renderer::draw_tree(QuadTree& tree) {
  
  // Draw the bounding box
  std::shared_ptr<BoundingBox> box = tree.get_box();
  draw_rectangle(box->x, box->y, box->width, box->height);

  // Check if the node has been divided
  if (!tree.check_divided()) {
    return;
  }

  // Draw the children
  std::array<std::shared_ptr<QuadTree>, 4> children = tree.get_children();
  for (auto& child : children) {
    draw_tree(*child);
  }

}

// Draw the points directly
void Renderer::draw_points(std::shared_ptr<vector<BodyGenerator::Body>>& points) {
  if (points->empty()) return;
  sf::VertexArray vertices(sf::Points, points->size());
  for (size_t i = 0; i < points->size(); ++i) {
      vertices[i].position = sf::Vector2f((*points)[i].x, (*points)[i].y);
      vertices[i].color = sf::Color::Green;
  }
  window.draw(vertices); // Single batch draw call
}
