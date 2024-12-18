#pragma once
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

// Forward declaration
class QuadTree;

class BodyGenerator {

  public: 

    // Define a body
    struct Body {
      double x;
      double y;
      double vx;
      double vy;
      double mass;
      QuadTree* node;
    };

    // Generate a bunch of bodies
    static std::shared_ptr<vector<Body>> generate_bodies(int number, double cx, double cy, double max_radius);


};