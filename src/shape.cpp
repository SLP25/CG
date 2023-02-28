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
#include <iostream>


Shape::Shape() { this->triangles = std::vector<Triangle>(); }

Shape::Shape(std::vector<Triangle> triangles) { this->triangles = triangles; }

Shape::Shape(std::string filePath) {
  std::ifstream file(filePath);
  int n;
  file >> n;
  this->triangles = std::vector<Triangle>();
  
  for (int i = 0; i < n; i++) {
    Point points[3];
    for (int j = 0; j < 3; j++) {
      float x, y, z;
      file >> x >> y >> z;
      points[j] = {x, y, z};
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
