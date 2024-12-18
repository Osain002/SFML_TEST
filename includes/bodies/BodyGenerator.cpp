#include <memory>
#include "BodyGenerator.h"

// Define the body generation function
std::shared_ptr<vector<BodyGenerator::Body>> BodyGenerator::generate_bodies(int number, double cx, double cy, double max_radius) {

  // Create the container 
  std::shared_ptr<vector<BodyGenerator::Body>> bodies = std::make_shared<vector<BodyGenerator::Body>>();
  bodies->reserve(number);

  for (int i=0; i<number; i++) {

    // Create a body
    BodyGenerator::Body body;
    body.x = max_radius * rand() / RAND_MAX;
    body.y = max_radius * rand() / RAND_MAX;;
    body.vx = 0.f;
    body.vy = 0.f;

    double mass = 2 * M_PI * rand() / RAND_MAX;
    body.mass = mass;

    // Add the body to the list of bodies
    bodies->push_back(body);
  }

  return bodies;
}