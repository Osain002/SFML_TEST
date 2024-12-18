#include <iostream>
#include <cmath>
#include <algorithm>
#include <execution>
#include "QuadTree.h"

// Construct the object
QuadTree::QuadTree(BoundingBox& box, int capacity): 
  bounding_box(std::make_shared<BoundingBox>(box)), 
  node_capacity(capacity) {
  points.reserve(node_capacity);
}

// Clear the tree
void QuadTree::clear(bool is_child) {

  // Clear the points
  points.clear();

  // Reset the children
  for (auto& child : children) {
    if (child) {
      child->clear(true);
      child.reset();
    }
  }

  // Reset the bounding boxes
  if (is_child) {
    bounding_box.reset();
  }

  divided = false;
}

// Insert a point
bool QuadTree::insert_point(const std::shared_ptr<BodyGenerator::Body>& body) {
  
  // Ensure the point fits in this node's box
  if (!bounding_box->contains_body(*body)) {
    return false;
  }

  // Check if this node has been divided
  if (divided) {
    for (auto& child : children) {
      if (child->insert_point(body)) {
        return true;
      }
    }
  }

  // Add the point to this node
  if (points.size() < node_capacity) {
    body->node = this;
    node_mass = body->mass;
    points.push_back(body);
    return true;
  }

  // Divide the region and push the current points
  // down to the children
  if (!divided) {
    divide();
  }

  // Push the point to a child node
  body->node = nullptr;
  
  for (auto& child : children) {
    if (child->insert_point(body)) {
      break;
    }
  }

  return true;
}

// Insert multiple points into the tree
void QuadTree::insert_points(std::shared_ptr<vector<BodyGenerator::Body>>& new_points) {
  for (const auto& point : *new_points) {
    insert_point(std::make_shared<BodyGenerator::Body>(point));
  }
}

// Get the distance from this point to the center of mass
double QuadTree::distance_from_center(BodyGenerator::Body& body) {
  const double dx = body.x - mass_center.x;
  const double dy = body.y - mass_center.y;
  const double d_squared = dx * dx + dy * dy;
  return sqrt(d_squared);
}

// Divide the node 
void QuadTree::divide() {

  node_mass = 0.0f;

  // The new box dimensions
  double new_width = 0.5 * bounding_box->width;
  double new_height = 0.5 * bounding_box->height;

  // Create the bounding boxes
  BoundingBox box_1(bounding_box->x, bounding_box->y, new_width, new_height);
  BoundingBox box_2(bounding_box->x + new_width, bounding_box->y, new_width, new_height);
  BoundingBox box_3(bounding_box->x, bounding_box->y + new_height, new_width, new_height);
  BoundingBox box_4(bounding_box->x + new_width, bounding_box->y + new_height, new_width, new_height);

  // Create the child nodes
  children[0] = std::make_unique<QuadTree>(box_1, node_capacity);
  children[1] = std::make_unique<QuadTree>(box_2, node_capacity);
  children[2] = std::make_unique<QuadTree>(box_3, node_capacity);
  children[3] = std::make_unique<QuadTree>(box_4, node_capacity);

  // Push the current points down to the children
  for (auto& point : points) {
    
    // Reset the points node
    point->node = nullptr;

    // Insert into the child nodes
    for(auto& child : children) {
      if (child->insert_point(point)) {
        break;
      }
    }
  }

  // Clear the points from this node
  points.clear();

  // Mark as divided
  divided = true;
}

// Print the full data structure
void QuadTree::print(int indent) {

  // Print general info
  std::string line(indent, ' ');
  std::cout << line << "BOX (" << bounding_box->x << ", " << bounding_box->y << ", " << bounding_box->width << ", " << bounding_box->height << ")" << endl;
  std::cout << line << "CAPACITY " << node_capacity << endl;
  
  // Print the points
  std::cout << line << "POINTS" << endl;
  for (auto& point : points) {
    if (point) {
      std::cout << line << "(" << point->x << " " << point->y << ")" << endl;
    } else {
      std::cout << line << "NULL" << endl;
    }
  }

  std::cout << "NODE MASS" << node_mass << endl;
  
  // Print the children
  for (auto& child : children) {
    if (child) {
      child->print(indent + 1);
    }
  }

}

//==== Private section

std::tuple<double, QuadTree::MassCenter&> QuadTree::propagate_mass_centers() {
  
  // If this is a leaf node with points
  if (!divided && !points.empty()) {
    node_mass = 0.0f;
    mass_center.x = 0.0f;
    mass_center.y = 0.0f;
    
    // Sum the mass and mass-weighted coordinates
    for (auto& point : points) {
      node_mass += point->mass;
      mass_center.x += point->x * point->mass;
      mass_center.y += point->y * point->mass;
    }

    // Normalize the center of mass by the total mass
    if (node_mass > 0) {
      mass_center.x /= node_mass;
      mass_center.y /= node_mass;
    }
    return std::tie(node_mass, mass_center);
  }

  // Otherwise, compute for child nodes
  double total_mass = 0.0f;
  double weighted_mass_center_x = 0.0f;
  double weighted_mass_center_y = 0.0f;

  for (auto& child : children) {

    if (child) {
      auto [child_mass, child_center] = child->propagate_mass_centers();
      total_mass += child_mass;
      weighted_mass_center_x += child_center.x * child_mass;
      weighted_mass_center_y += child_center.y * child_mass;
    }
  }

  // Update this node's mass and center of mass
  node_mass = total_mass;
  if (node_mass > 0) {
    mass_center.x = weighted_mass_center_x / node_mass;
    mass_center.y = weighted_mass_center_y / node_mass;
  } else {
    mass_center.x = 0.0f;
    mass_center.y = 0.0f;
  }

  return std::tie(node_mass, mass_center);
}