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
#include <map>
#include <iostream>


Shape::Shape() { 
  this->points = std::vector<Point>();
  this->trianglesByPos = std::vector<TriangleByPosition>();
  }


Shape::Shape(std::vector<Triangle> triangles) {
  this->points = std::vector<Point>();
  this->trianglesByPos = std::vector<TriangleByPosition>();
  std::map<Point,int> pointsFound;
  Point points[3];
  int pos[3];

  for (Triangle &triangle : triangles){
    points[0] = std::get<0>(triangle);
    points[1] = std::get<1>(triangle);
    points[2] = std::get<2>(triangle);
    for (int i=0;i<3;i++){
      auto it = pointsFound.find(points[i]);
      if ( it == pointsFound.end() ){
        this->points.push_back(points[i]);
        pointsFound[points[i]]=this->points.size()-1;
        pos[i]=pointsFound[points[i]];
      }else{
        pos[i]=it->second;
      }
    }
    this->trianglesByPos.push_back({pos[0],pos[1],pos[2]});
  }
}

Shape::Shape(std::string filePath) {
  std::ifstream file(filePath);
  int n;
  file >> n;
  this->points = std::vector<Point>();
  for (int i = 0; i < n; i++) {
    float x, y, z;
    file >> x >> y >> z;
    this->points.push_back({x,y,z});
  }
  file >> n;
  this->trianglesByPos = std::vector<TriangleByPosition>();
  for (int i = 0; i < n; i++) {
    int t1, t2, t3;
    file >> t1 >> t2 >> t3;
    this->trianglesByPos.push_back({t1,t2,t3});
  }
  file.close();
}

void Shape::draw(){
  for (TriangleByPosition &triangle : this->trianglesByPos){
      Point p1 = this->points[std::get<0>(triangle)];
      Point p2 = this->points[std::get<1>(triangle)];
      Point p3 = this->points[std::get<2>(triangle)];
      
      glBegin(GL_TRIANGLES);
  
      glVertex3f(std::get<0>(p1),std::get<1>(p1),std::get<2>(p1));
      glVertex3f(std::get<0>(p2),std::get<1>(p2),std::get<2>(p2));
      glVertex3f(std::get<0>(p3),std::get<1>(p3),std::get<2>(p3));
      glEnd();
  }
}

bool Shape::exportToFile(std::string filePath) {
  std::ofstream file(filePath);
  file << this->points.size() << std::endl;
  for (Point p : this->points)
    file << std::get<0>(p) << " " << std::get<1>(p) << " " << std::get<2>(p) << std::endl;
  file << this->trianglesByPos.size() << std::endl;
  for (TriangleByPosition pos : this->trianglesByPos)
    file << std::get<0>(pos) << " " << std::get<1>(pos) << " " << std::get<2>(pos) << std::endl;
  file.close();
  return true;
}
