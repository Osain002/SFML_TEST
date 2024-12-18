#pragma once
#include "../bodies/BodyGenerator.h"

class BoundingBox {

  public:

    // Default constructor
    BoundingBox() : x(0.0f), y(0.0f), width(0.0f), height(0.0f) {}

    // Parameterized constructor
    BoundingBox(double box_x, double box_y, double box_width, double box_height);

    // Check if the box contains this body
    bool contains_body(BodyGenerator::Body body);

    // Retreive the variables
    double x, y, width, height;

};