/**
 * @file shape.hpp
 *
 * @brief File defining the @link Shape class
 */
#pragma once
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include "utils.hpp"

typedef std::tuple<int,int,int> TriangleByPosition;

/**
 * @brief A class representing a shape in 3D space.
 *
 * A shape can be anything from a sphere, a plane, a teapot or any
 * more complicated 2D/3D object
 */
class Shape {
public:
  /**
   * @brief Default constructor
   */
  Shape();
  Shape(std::vector<Triangle> triangles);
  Shape(std::string filePath);

  /**
   * @brief Exports the shape to a 3D file
   *
   * The file follows the Wavefront .obj
   * (https://en.wikipedia.org/wiki/Wavefront_.obj_file) file format
   *
   * @param filePath the path of the file to write to
   *
   * @return whether the operation was successful
   */
  bool exportToFile(std::string filePath);

  /**
   * @brief Draws the shape by calling glut's static functions. No color or texture
   * is set, only the shape is drawn.
   * 
   */
  void draw();

private:
  /**
   * The vertices of the shape
   */
  std::vector<Point> points;
  std::vector<TriangleByPosition> trianglesByPos;
};
