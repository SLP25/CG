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

// TODO: Refactor to class if needed

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
   * The file follows the <a
   * href="https://en.wikipedia.org/wiki/Wavefront_.obj_file">Wavefront .obj</a>
   * file format
   *
   * @param filePath the path of the file to write to
   *
   * @return whether the operation was successful
   */
  bool exportToFile(std::string filePath);
  void draw();

private:
  /**
   * The vertices of the shape
   */
  std::vector<Point> points;
  std::vector<TriangleByPosition> trianglesByPos;
};
