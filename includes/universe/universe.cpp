#include <iostream>
#include <thread>
#include <cmath>
#include "Universe.h"

Universe::Universe(int num_bodies, Renderer renderer) : renderer(renderer) {
  BoundingBox box(0, 0, 500, 500);
  tree = std::make_unique<QuadTree>(box, 1);
  bodies = BodyGenerator::generate_bodies(num_bodies, 250.0f, 250.0f, 500.0f);
  tree->insert_points(bodies);
  tree->propagate_mass_centers();
}

// Evolve the system
void Universe::evolve(double dt) {

  std::vector<std::thread> threads;
  
  // Number of threads
  const int num_threads = std::thread::hardware_concurrency();  // Use the available CPU cores
  int bodies_per_thread = bodies->size() / num_threads;

  // Launch threads to update positions
  for (int i = 0; i < num_threads; ++i) {
    int start_index = i * bodies_per_thread;
    int end_index = (i == num_threads - 1) ? bodies->size() : start_index + bodies_per_thread;
    threads.emplace_back([this, start_index, end_index, dt]() {
      for (int i=start_index; i<end_index; i++) {
        update_positions(tree, (*this->bodies)[i], 1, dt);
      }
    });
  }
  
  // Join threads
  for (auto& thread : threads) {
    thread.join();
  }
  
  render();  
  tree->clear();
  tree->insert_points(bodies);
  tree->propagate_mass_centers();
}

// Render the current state of the system
void Universe::render() {
  // renderer.draw_tree(*tree);
  renderer.draw_points(bodies);
}

void Universe::print_bodies() {
  for (auto& body : *bodies) {
    std::cout << "----------" << endl;
    std::cout << body.x << endl;
    std::cout << body.y << endl;
    std::cout << body.vx << endl;
    std::cout << body.vy << endl;
  }
}

//==== Private section

// Calculate the force
std::vector<double> Universe::calculate_force(double x1, double y1, double x2, double y2, double m1, double m2) {
  
  std::vector<double> result;
  result.reserve(2);

  if (x1 == x2 && y1 == y2) {
    return result;
  }

  // Calculate the coordinate differences
  double dx = x2 - x1;
  double dy = y2 - y1;

  // Calculate the force magnitude
  double distance_squared = dx * dx + dy * dy;
  double distance = sqrt(distance_squared);
  double force_magnitude = (G * m1 * m2) / (distance_squared + 1);

  result[0] = (dx * force_magnitude) / distance;
  result[1] = (dy * force_magnitude) / distance;

  return result;
}

// Update particle position
void Universe::update_particle(BodyGenerator::Body& body, std::vector<double>& force, double dt) {
  body.vx += dt * force[0] / body.mass;
  body.vy += dt * force[1] / body.mass;
  body.x += body.vx * dt;
  body.y += body.vy * dt;
}

// Update the positions
void Universe::update_positions(std::shared_ptr<QuadTree> node, BodyGenerator::Body& body, double tolerance, double dt) {

  // Ensure the node has mass
  if (node->node_mass == 0) {
    return;
  }

  // Calculate the distances
  double s = node->get_box().width;
  double d = node->distance_from_center(body);

  // Update the position
  if (!node->divided || s/d < tolerance) {
    std::vector<double> force = calculate_force(body.x, body.y, node->mass_center.x, node->mass_center.y, body.mass, node->node_mass);
    return update_particle(body, force, dt);
  }

  // Propagate to the children
  for (auto& child : node->children) {
    if (child != nullptr) {
      update_positions(child, body, tolerance, dt);
    }
  }

}