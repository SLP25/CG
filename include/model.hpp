#pragma once

#include "parser.hpp"
#include "shape.hpp"

/**
 * @brief Represents a model that gets rendered into the world
 *
 */
class Model {
private:
  Shape shape;
  std::tuple<float, float, float> color;
public:
  /**
   * @brief Constructs a new Model object from a given Shape
   *
   * @param shape The given shape
   */
  Model(Shape shape);
  Model(XMLParser parser);

  /**
   * @brief Draws the model by calling glut's static functions
   *
   */
  void draw();

private:
  std::tuple<float, float, float> parseHexColor(std::string color);
};
