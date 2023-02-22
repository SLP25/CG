/**
 * @file shapegenerator.cpp
 *
 * @brief File implementing functions used to generate the different shapes
 */

#include "shapegenerator.hpp"
#include <iostream>
#include <map>

std::unique_ptr<Shape> generateSphere(float radius, int slices, int stacks) {
  std::cout << (radius) / (slices + stacks); // So para compilar, remover depois
  return std::make_unique<Shape>();
}

std::unique_ptr<Shape> generateCone(float radius, float height, int slices,
                                    int stacks) {
  std::cout << (radius + height) /
                   (slices + stacks); // So para compilar, remover depois
  return std::make_unique<Shape>();
}

std::unique_ptr<Shape> generateCube(float length, int divisions) {
  std::cout << length / divisions; // So para compilar, remover depois
  return std::make_unique<Shape>();
}

inline void generateSquare(Point p1, Point p2, Point p3, Point p4,
                           std::vector<Triangle> &triangles) {
  triangles.push_back({p1, p2, p3});
  triangles.push_back({p1, p3, p4});
}

std::unique_ptr<Shape> generatePlane(float length, int divisions) {
  /*

  In order to generate a plane with length l and d divisions, we do the
  following:

  1. Compute all the relevant points in the plane
  2. Create a mapping between these points and their indices in the
  vector that stores them. This will be useful when computing the faces
  3. Compute the vertices belonging to each face. Use the reverse mapping
  to store the indices of all vertices of a face. They must be generated
  in counter-clockwise fashion

  */
  auto triangles = std::vector<Triangle>();

  float mid = length / 2.0;
  float step = length / divisions;

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({-mid + i * step, 0, -mid + j * step},
                     {-mid + i * step, 0, -mid + (j + 1) * step},
                     {-mid + (i + 1) * step, 0, -mid + (j + 1) * step},
                     {-mid + (i + 1) * step, 0, -mid + j * step}, triangles);
    }
  }

  return std::make_unique<Shape>(triangles);
}
