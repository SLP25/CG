#pragma once

/**
 * @file model.hpp
 * @brief File declaring the #Model class, used to represent the models in the scene
*/

#include "parser.hpp"
#include "shape.hpp"

/**
 * @brief Represents a model that gets rendered into the world
 *
 */
class Model {
private:
  /**
   * @brief The shape of the model
  */
  std::shared_ptr<Shape> shape;

  /**
   * @brief The color of the model in RGB.
   * 
   * The values range from [0-1]. 1 corresponds to all of that color
   * (255) and 0 to no color (0).
   * 
   * 0 -> the red part of the color
   * 1 -> the green part of the color
   * 2 -> the blue part of the color
  */
  std::tuple<float, float, float> color;
public:
  /**
   * @brief Constructs a new Model object from a given Shape
   *
   * @param shape The given shape
   */
  Model(Shape shape, std::tuple<float,float,float> color);
  Model(XMLParser parser);

  /**
   * @brief Draws the model by calling glut's static functions
   *
   */
  void draw();
};
