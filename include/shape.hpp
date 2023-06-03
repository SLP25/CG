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
#include <geometry.hpp>
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
  static void reloadShapes();
  static void initShapes();

  /**
   * @brief Default constructor
   */
  Shape();
  Shape(const std::vector<Triangle>& triangles, const std::vector<Vector>& normals,  const std::vector<Point2D>& textureCoordinates);
  Shape(const std::vector<Triangle>& triangles); //the normals are extapolated from the triangles
  Shape(std::vector<Point> points, std::vector<Vector> normals, std::vector<Point2D> textures, std::vector<TriangleByPosition> trianglesByPos);
  Shape(const std::vector<Triangle>& triangles, const std::map<Point, Point2D>& textureCoordinates);

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
   * @brief Returns a copy of the bounding box of the shape
   * 
   * @return a copy of the bounding box of the shape
   */
  BoundingBox getBoundingBox();

  /**
   * @brief Draws the shape by calling glut's static functions. No color or texture
   * is set, only the shape is drawn.
   * 
   */
  void draw();

private:
  /**
   * @brief Constructs from the given file
   * 
   * @param filePath the path of the 3D file
  */
  Shape(std::string filePath);

private:
  /**
   * @brief Cache of #Shape from file paths. Implemented to avoid reading from files multiple times
  */
  static std::map<std::string, std::shared_ptr<Shape>> cache;
  
  /**
   * @brief The vertices of the shape
   */
  std::vector<Point> points;

  /**
   * @brief The normals for the vertices
  */
  std::vector<Vector> normals;
  
  /**
   * @brief The textures for the points of the shape. The key is the coordinates of the 
   * vertex, and the value is the texture coordinate of that vertex
  */
  std::vector<Point2D> textures;

  /**
   * @brief The axis-aligned bounding box of the shape
  */
  BoundingBox boundingBox;

  GLuint vbo_points;
  GLuint vbo_normals;
  GLuint vbo_textures;

  /**
   * @brief The triangles of the shape. For the i-th triangle, the tuple corresponds to
   * the index of the points in the points vector, following the right hand rule
  */
  std::vector<TriangleByPosition> trianglesByPos;
};
