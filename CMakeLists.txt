cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(/opt/homebrew/Cellar/sfml/2.6.2/include)

# Link directories
link_directories(/opt/homebrew/Cellar/sfml/2.6.2/lib)

# Add source files
set(
  SOURCES 
  main.cpp 
  includes/bodies/BodyGenerator.cpp
  includes/qtree/BoundingBox.cpp
  includes/qtree/QuadTree.cpp
  includes/renderer/Renderer.cpp
  includes/universe/Universe.cpp
)

# Add the executable
add_executable(prog ${SOURCES})

# Link SFML libraries
target_link_libraries(prog sfml-window sfml-graphics sfml-system)
