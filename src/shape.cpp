/**
 * @file shape.cpp
 *
 * @brief File implementing the @link Shape class
 */

#include "shape.hpp"
#include <fstream>
#include <tuple>

Shape::Shape() { this->triangles = std::vector<Triangle>(); }

Shape::Shape(std::vector<Triangle> triangles) { this->triangles = triangles; }

Shape::Shape(std::string filePath) {
  this->triangles = std::vector<Triangle>();
  std::ifstream file(filePath);
  int n;
  file >> n;
  for (int i = 0; i < n; i++) {
    auto points = std::vector<Point>();
    for (int j = 0; j < 3; j++) {
      float x, y, z;
      file >> x >> y >> z;
      points.push_back({x, y, z});
    }

    this->triangles.push_back({points[0], points[1], points[2]});
  }
  file.close();
}

bool Shape::exportToFile(std::string filePath) {
  std::ofstream file(filePath);
  file << this->triangles.size() << std::endl;
  for (Triangle t : this->triangles) {
    file << std::get<0>(std::get<0>(t)) << " " << std::get<1>(std::get<0>(t))
         << " " << std::get<2>(std::get<0>(t)) << std::endl;
    file << std::get<0>(std::get<1>(t)) << " " << std::get<1>(std::get<1>(t))
         << " " << std::get<2>(std::get<1>(t)) << std::endl;
    file << std::get<0>(std::get<2>(t)) << " " << std::get<1>(std::get<2>(t))
         << " " << std::get<2>(std::get<2>(t)) << std::endl;
  }

  file.close();
  return true;
}
