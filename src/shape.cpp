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
#include <iostream>
#include <sstream>
#include "exceptions/invalid_xml_file.hpp"
#include <map>
#include <tuple>

Shape::Shape() {}

Shape::Shape(std::vector<Triangle> triangles) {
  /**
   * @brief Creates a shape givel all triangles it is made of
   * @param triangles a vector containing the triangles that make up the shape
   */
  //points found in the vector of triangles and the position they will be stored in the points vector
  std::map<Point, int> pointsFound; 
  Point points[3];
  int pos[3];
  
  for (Triangle &triangle : triangles) {
    points[0] = std::get<0>(triangle);
    points[1] = std::get<1>(triangle);
    points[2] = std::get<2>(triangle);
    for (int i = 0; i < 3; i++) {
      auto it = pointsFound.find(points[i]);
      if (it == pointsFound.end()) { // if the point wasn't found yet
        this->points.push_back(points[i]); //add point to points vector
        pointsFound[points[i]] = this->points.size() - 1; //add to the map where it is stored
        pos[i] = pointsFound[points[i]];
      } else {
        pos[i] = it->second;
      }
    }
    //save the triangle as the positions of the points in the vector of points
    this->trianglesByPos.push_back({pos[0], pos[1], pos[2]});
  }
}

Shape::Shape(std::string filePath) {
  /**
   * @brief Read a shape from a file
   * @param filePath the path to the file containing the shape
   */
  std::ifstream file(filePath); //open the file

  if (!file) {

    std::stringstream exception_message;
    exception_message << "XMLParser@model: The file '" << filePath
                      << "' does not exist.";
    throw InvalidXMLStructure(exception_message.str());
  }

  int n;
  file >> n; //read number of points
  for (int i = 0; i < n; i++) {
    float x, y, z;
    file >> x >> y >> z; // read x,y,z for each point
    this->points.push_back({x, y, z}); // add the point to vector of points
  }
  file >> n; // read number of triangles
  for (int i = 0; i < n; i++) {
    int t1, t2, t3;
    file >> t1 >> t2 >> t3; // read the position of each point of the triangle in the vector of points
    this->trianglesByPos.push_back({t1, t2, t3}); // add the triangle to the vector of trianglesByPos
  }
  file.close();//close the file
}

void Shape::draw() {
  /**
   * @brief Draw the shape in the window
   * 
   */
  for (TriangleByPosition &triangle : this->trianglesByPos) {
    Point p1 = this->points[std::get<0>(triangle)];
    Point p2 = this->points[std::get<1>(triangle)];
    Point p3 = this->points[std::get<2>(triangle)];

    glBegin(GL_TRIANGLES);

    glVertex3f(std::get<0>(p1), std::get<1>(p1), std::get<2>(p1));
    glVertex3f(std::get<0>(p2), std::get<1>(p2), std::get<2>(p2));
    glVertex3f(std::get<0>(p3), std::get<1>(p3), std::get<2>(p3));
    glEnd();
  }
}

bool Shape::exportToFile(std::string filePath) {
  /**
   * @brief Stores a shape inside a file
   * @param filePath the path to the file to write the shape to
   */
  std::ofstream file(filePath);
  file << this->points.size() << std::endl;//write the number of points
  for (Point p : this->points)
    file << std::get<0>(p) << " " << std::get<1>(p) << " " << std::get<2>(p)
         << std::endl; // for each point write its x,y,z coordenates
  file << this->trianglesByPos.size() << std::endl;//write the number of triangles
  for (TriangleByPosition pos : this->trianglesByPos)
    file << std::get<0>(pos) << " " << std::get<1>(pos) << " "
         << std::get<2>(pos) << std::endl; // for each triangle write the position in the points vector of the points that compose the triangle
  file.close();
  return true;
}
