#pragma once
#include "../bodies/BodyGenerator.h"
#include "../qtree/QuadTree.h"
#include "../renderer/Renderer.h"

class Universe {

  public:
    Universe(int num_bodies, Renderer renderer);

    // Step forward
    void step(double dt);

    // Evolve the system
    void evolve(double dt);

    // Render the current state
    void render();

    // Print the bodies
    void print_bodies();

    struct Vector {
      double x;
      double y;
    };

  private:

    // Calculate the forces on each particle
    Universe::Vector calculate_force(double x1, double y1, double x2, double y2, double m1, double m2);

    // Update a particles position
    void update_particle(BodyGenerator::Body& body, Universe::Vector force, double dt);

    // Update the position of each particle
    void update_positions(std::shared_ptr<QuadTree> node, BodyGenerator::Body& body, double tolerance, double dt);
    
    // Physical values
    double G = 6.67e-1;

    // Data structures
    std::shared_ptr<vector<BodyGenerator::Body>> bodies;
    std::shared_ptr<QuadTree> tree;

    // The renderer
    Renderer renderer;

};