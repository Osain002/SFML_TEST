#include "BoundingBox.h"

// Construct the box
BoundingBox::BoundingBox(double box_x, double box_y, double box_width, double box_height) {
  x = box_x;
  y = box_y;
  width = box_width;
  height = box_height;
}

// Check if the box contains this body
bool BoundingBox::contains_body(BodyGenerator::Body body) {
  return (body.x > x && body.x < x + width &&
  body.y > y  && body.y < y + width);
}