/**
 * @file shapegenerator.cpp
 *
 * @brief File implementing functions used to generate the different shapes
 */

#include "shapegenerator.hpp"
#include <iostream>
#include <math.h>
#include <map>
#include "utils.hpp"

std::vector<Point> generateCircle(Point center, float radius, int slices) {
  std::vector<Point> ans;
  
  for (int i = 0; i < slices; i++) {
    float angle = 2 * PI * i / slices;
    Vector v = { radius * cos(angle), 0, radius * sin(angle) };
    ans.push_back(addVector(center, v));
  }

  return ans;
}

void generateSquare(Point p1, Point p2, Point p3, Point p4,
                           std::vector<Triangle> &triangles) {
  triangles.push_back({p1, p2, p3});
  triangles.push_back({p1, p3, p4});
}

void generatePolygon(std::vector<Point> vertices, std::vector<Triangle> &triangles) {
  for (int i = 2; i < (int)vertices.size(); i++)
    triangles.push_back({ vertices.at(0), vertices.at(i - 1), vertices.at(i) });
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
  std::vector<Triangle> triangles;

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

std::unique_ptr<Shape> generateCube(float length, int divisions) {
  std::vector<Triangle> triangles;

  float mid = length / 2.0;
  float step = length / divisions;

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({-mid, -mid + i * step, -mid + j * step},
                      {-mid, -mid + i * step, -mid + (j + 1) * step},
                      {-mid, -mid + (i + 1) * step, -mid + (j + 1) * step},
                      {-mid, -mid + (i + 1) * step, -mid + j * step}, triangles);
    }
  }

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({mid, -mid + (i + 1) * step, -mid + j * step},
                     {mid, -mid + (i + 1) * step, -mid + (j + 1) * step},
                     {mid, -mid + i * step, -mid + (j + 1) * step},
                     {mid, -mid + i * step, -mid + j * step}, triangles);
    }
  }

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({-mid + (i + 1) * step, -mid, -mid + j * step},
                     {-mid + (i + 1) * step, -mid, -mid + (j + 1) * step},
                     {-mid + i * step, -mid, -mid + (j + 1) * step},
                     {-mid + i * step, -mid, -mid + j * step}, triangles);
    }
  }

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({-mid + i * step, mid, -mid + j * step},
                     {-mid + i * step, mid, -mid + (j + 1) * step},
                     {-mid + (i + 1) * step, mid, -mid + (j + 1) * step},
                     {-mid + (i + 1) * step, mid, -mid + j * step}, triangles);
    }
  }

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({-mid + i * step, -mid + j * step, -mid},
                     {-mid + i * step, -mid + (j + 1) * step, -mid},
                     {-mid + (i + 1) * step, -mid + (j + 1) * step, -mid},
                     {-mid + (i + 1) * step, -mid + j * step, -mid}, triangles);
    }
  }

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({-mid + (i + 1) * step, -mid + j * step, mid},
                     {-mid + (i + 1) * step, -mid + (j + 1) * step, mid},
                     {-mid + i * step, -mid + (j + 1) * step, mid},
                     {-mid + i * step, -mid + j * step, mid}, triangles);
    }
  }

  return std::make_unique<Shape>(triangles);
}


std::unique_ptr<Shape> generateCone(float radius, float height, int slices,
                                    int stacks) {
  
  std::vector<Triangle> ans;
  std::vector<Point> prev;

  for (int i = 0; i <= stacks; i++) {
    float r = radius * (stacks - i) / stacks;

    std::vector<Point> cur = generateCircle({0, height * i / stacks, 0}, r, slices);

    if (i == 0)
      generatePolygon(cur, ans);
    else
      for (int j = 0; j < slices; j++)
        generateSquare(prev.at((j + 1) % slices), prev.at(j), cur.at(j), cur.at((j + 1) % slices), ans);

    prev = cur;
  }

  return std::make_unique<Shape>(ans);
}


std::unique_ptr<Shape> generateSphere(float radius, int slices, int stacks) {
  std::vector<Triangle> ans;
  std::vector<Point> prev;

  for (int i = 0; i <= stacks; i++) {
    float h = radius * (2.0f * i / stacks - 1);
    float r = sqrt(radius * radius - h * h);

    std::vector<Point> cur = generateCircle({0, h, 0}, r, slices);

    if (i != 0)
      for (int j = 0; j < slices; j++)
        generateSquare(prev.at((j + 1) % slices), prev.at(j), cur.at(j), cur.at((j + 1) % slices), ans);

    prev = cur;
  }

  return std::make_unique<Shape>(ans);
}