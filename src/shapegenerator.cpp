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
  auto points = std::vector<Point>();
  auto reversePoints = std::map<Point, int>();
  auto faces = std::multimap<int, int>();

  float mid = length / 2.0;
  float step = length / divisions;
  int idx = 0;

  for (float x = -mid; x <= mid; x += step) {
    for (float z = -mid; z <= mid; z += step) {
      points.push_back({x, 0, z});
      reversePoints[{x, 0, z}] = idx;
      idx++;
    }
  }

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      faces.insert({i * divisions + j,
                    reversePoints[{-mid + i * step, 0, -mid + j * step}]});
      faces.insert(
          {i * divisions + j,
           reversePoints[{-mid + i * step, 0, -mid + (j + 1) * step}]});
      faces.insert(
          {i * divisions + j,
           reversePoints[{-mid + (i + 1) * step, 0, -mid + (j + 1) * step}]});
      faces.insert(
          {i * divisions + j,
           reversePoints[{-mid + (i + 1) * step, 0, -mid + j * step}]});
    }
  }

  return std::make_unique<Shape>(points, faces);
}
