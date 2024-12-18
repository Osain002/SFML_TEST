#pragma once
#include <vector>
#include <array>
#include <memory>
#include <tuple>
#include <algorithm>
#include <execution>
#include "BoundingBox.h"
#include "../bodies/BodyGenerator.h"

class QuadTree {

  public:
    
    // Center of mass
    struct MassCenter {
      double x = 0.0f;
      double y = 0.0f;
    };

    // Construct the quadtree
    QuadTree(BoundingBox& box, int capacity);

    // Clear the tree structure
    void clear(bool is_child=false);

    // Insert a point into the tree
    bool insert_point(const std::shared_ptr<BodyGenerator::Body>& body);

    // Insert multiple point into the tree
    void insert_points(std::shared_ptr<vector<BodyGenerator::Body>>& new_points);

    // Divide the node
    void divide();

    // Get the distance from this point to the center of mass
    double distance_from_center(BodyGenerator::Body& body);

    // Propogate the mass centers
    std::tuple<double, MassCenter&> propagate_mass_centers();
    
    // Print the data structure
    void print(int indent=0);
    
    // Get the bounding box for this node
    const BoundingBox& get_box() const {
      return bounding_box;
    }

    // Get the children
    std::array<std::shared_ptr<QuadTree>, 4> get_children() const {
      return children;
    }

    // Check if this node is divided
    bool check_divided() const {
      return divided;
    }

    double node_mass = 0.0f;
    MassCenter mass_center;
    std::array<std::shared_ptr<QuadTree>, 4> children;
    bool divided = false;
    int node_capacity;

  private:

    // The bounding box for the node
    BoundingBox bounding_box;

    // The points currently in this node
    vector<std::shared_ptr<BodyGenerator::Body>> points;

};