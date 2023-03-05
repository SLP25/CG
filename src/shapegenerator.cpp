/**
 * @file shapegenerator.cpp
 *
 * @brief File implementing functions used to generate the different shapes
 */

#include "shapegenerator.hpp"
#include "utils.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>

// displacement is a fraction of the angle of slicing. 0.5 displacement means
// the circle is offset by PI/slices rad
std::vector<Point> generateCircle(Point center, float radius, int slices,
                                  float displacement) {
  std::vector<Point> ans;

  for (int i = 0; i < slices; i++) {
    float angle = 2 * M_PI * (i + displacement) / slices;
    Vector v = {radius * cos(angle), 0, radius * sin(angle)};
    ans.push_back(addVector(center, v));
  }

  return ans;
}

void generateSquare(Point p1, Point p2, Point p3, Point p4,
                    std::vector<Triangle> &triangles) {
  triangles.push_back({p1, p2, p3});
  triangles.push_back({p1, p3, p4});
}

void generatePolygon(std::vector<Point> vertices,
                     std::vector<Triangle> &triangles) {
  for (int i = 2; i < (int)vertices.size(); i++)
    triangles.push_back({vertices.at(0), vertices.at(i - 1), vertices.at(i)});
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

std::unique_ptr<Shape> generateCylinder(float radius, float height,
                                        int slices) {
  std::vector<Point> top = generateCircle({0, height, 0}, radius, slices, 0);
  std::vector<Point> bottom = generateCircle({0, 0, 0}, radius, slices, 0.5);
  std::vector<Triangle> ans;

  for (int i = 0; i < slices; i++) {
    int i2 = (i + 1) % slices;
    ans.push_back({top[i], top[i2], bottom[i]});
    ans.push_back({bottom[i2], bottom[i], top[i2]});
  }

  reverse(top.begin(), top.end());
  generatePolygon(top, ans);
  generatePolygon(bottom, ans);

  return std::make_unique<Shape>(ans);
}

std::unique_ptr<Shape> generateCone(float radius, float height, int slices,
                                    int stacks) {

  std::vector<Triangle> ans;
  std::vector<Point> prev;

  for (int i = 0; i <= stacks; i++) {
    float r = radius * (stacks - i) / stacks;

    std::vector<Point> cur =
        generateCircle({0, height * i / stacks, 0}, r, slices, 0);

    if (i == 0)
      generatePolygon(cur, ans);
    else
      for (int j = 0; j < slices; j++)
        generateSquare(prev.at((j + 1) % slices), prev.at(j), cur.at(j),
                       cur.at((j + 1) % slices), ans);

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

    std::vector<Point> cur = generateCircle({0, h, 0}, r, slices, 0);

    if (i != 0)
      for (int j = 0; j < slices; j++)
        generateSquare(prev.at((j + 1) % slices), prev.at(j), cur.at(j),
                       cur.at((j + 1) % slices), ans);

    prev = cur;
  }

  return std::make_unique<Shape>(ans);
}

std::unique_ptr<Shape> generateFromObj(std::string srcFile) {
  // We add one dummy vertex at the start as to shift indices by 1,
  // as vectors start at 0 but OBJ files start at 1
  auto vertices = std::vector<Point>({{0, 0, 0}});
  auto triangles = std::vector<Triangle>();

  std::ifstream file = std::ifstream(srcFile);

  std::string line = "";
  const std::regex vertex(R"(v +(-?\d+\.?\d*) (-?\d+\.?\d*) (-?\d+\.?\d*))");
  const std::regex face(
      R"(f +((\d+)(\/?\d*\/?\d*)? ){2,}((\d+)(\/?\d*\/?\d*)?))");
  const std::regex face_vertice(R"((\d+)(\/?\d*\/?\d*))");

  while (std::getline(file, line)) {
    std::smatch match;
    if (std::regex_search(line, match, vertex)) { // Is a vertex
      float x = std::stof(match[1].str());
      float y = std::stof(match[2].str());
      float z = std::stof(match[3].str());
      vertices.push_back({x, y, z});
    } else if (std::regex_search(line, match, face)) { // Is a face
      auto temp = std::vector<Point>();

      while (std::regex_search(line, match, face_vertice)) {
        int index = std::stoi(match[1]);
        temp.push_back(vertices[index]);
        line = match.suffix().str();
      }

      generatePolygon(temp, triangles);
    } // We ignore everything else
  }
  file.close();
  return std::make_unique<Shape>(triangles);
}

std::unique_ptr<Shape> generateDonut(float radius, float length, float height,
                                     int stacks, int slices) {
  std::vector<Point> *circles[2];
  circles[0] = new std::vector<Point>[stacks];
  circles[1] = new std::vector<Point>[stacks];

  const float stack_height = height / (float)stacks;
  const float min_y = -height / 2.0;
  const float angle_step = 2 * M_PI / slices;
  for (int i = 0; i < stacks; i++) {
    float y = min_y + stack_height * i;
    float horizontal_gap =
        sqrt(1 - (4 * y * y) / (height * height)) * (length / 2.0);

    for (int j = 0; j < slices; j++) {
      float x = (radius - horizontal_gap) * cos(angle_step * j);
      float z = (radius - horizontal_gap) * sin(angle_step * j);

      circles[0][i].push_back({x, y, z});

      x = (radius + horizontal_gap) * cos(angle_step * j);
      z = (radius + horizontal_gap) * sin(angle_step * j);

      circles[1][i].push_back({x, y, z});
    }
  }

  std::vector<Triangle> triangles = std::vector<Triangle>();

  for (int i = 0; i < stacks - 1; i++) {
    for (int j = 0; j < slices; j++) {
      Point p1 = circles[0][i][j];
      Point p2 = circles[0][i][(j + 1) % slices];
      Point p3 = circles[0][i + 1][j];
      Point p4 = circles[0][i + 1][(j + 1) % slices];

      triangles.push_back({p3, p2, p1});
      triangles.push_back({p4, p3, p2});

      p1 = circles[1][i][j];
      p2 = circles[1][i][(j + 1) % slices];
      p3 = circles[1][i + 1][j];
      p4 = circles[1][i + 1][(j + 1) % slices];

      triangles.push_back({p3, p1, p2});
      triangles.push_back({p3, p4, p2});
    }
  }

  delete[] circles[0];
  delete[] circles[1];

  return std::make_unique<Shape>(triangles);
}
