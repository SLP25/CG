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
#include "glut.hpp"

typedef std::tuple<int,int,int> TriangleByPosition;

/**
 * @brief A class representing a shape in 3D space.
 *
 * A shape can be anything from a sphere, a plane, a teapot or any
 * more complicated 2D/3D object
 */
class Shape {
public:
  static std::shared_ptr<Shape> fetchShape(std::string filePath);
  static void initShapes();

  /**
   * @brief Default constructor
   */
  Shape();
  Shape(std::vector<Triangle> triangles); //the normals are extapolated from the triangles
  Shape(std::vector<Point> points, std::vector<Vector> normals, std::vector<TriangleByPosition> trianglesByPos);

  /**
   * @brief Copy constructor
   * 
   * @param shape the Shape to copy
  */
  Shape(const Shape& shape);

  /**
   * @brief Move constructor
   * 
   * @param shape The Shape to move
  */
  Shape(Shape&& shape);

  /**
   * @brief Destructor (disposes vbo)
  */
  ~Shape();

  /**
   * @brief Assignment operator
   * @param shape the Shape to assign this as
  */
  Shape& operator=(const Shape& shape);

  /**
   * @brief Assignment operator
   * @param shape the Shape to assign this as
  */
  Shape& operator=(Shape&& shape);

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
   * @brief Initializes the Shape's VBO
   * 
   */
  void initialize();

  /**
   * @brief Draws the shape by calling glut's static functions. No color or texture
   * is set, only the shape is drawn.
   * 
   */
  void draw();

private:
  static std::map<std::string, std::shared_ptr<Shape>> cache;
  
  /**
   * The vertices of the shape
   */
  std::vector<Point> points;
  std::vector<Vector> normals;
  std::vector<TriangleByPosition> trianglesByPos;

  GLuint vbo_points;
  GLuint vbo_normals;

  Shape(std::string filePath);
};
