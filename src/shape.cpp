/**
 * @file shape.cpp
 *
 * @brief File implementing the @link Shape class
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

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

void Shape::draw(){
  for (Triangle &triangle : this->triangles){
      Point p1 = std::get<0>(triangle);
      Point p2 = std::get<1>(triangle);
      Point p3 = std::get<2>(triangle);
      glBegin(GL_TRIANGLES);
      glColor3f(0.1, 0.2, 0.3);
      glVertex3f(std::get<0>(p1),std::get<1>(p1),std::get<2>(p1));
      glVertex3f(std::get<0>(p2),std::get<1>(p2),std::get<2>(p2));
      glVertex3f(std::get<0>(p3),std::get<1>(p3),std::get<2>(p3));
      glEnd();
    }
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
