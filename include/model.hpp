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
  Shape shape;

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
  Model(Shape shape);
  Model(XMLParser parser);

  /**
   * @brief Draws the model by calling glut's static functions
   *
   */
  void draw();

private:
  /**
   * Casts an hexadecimal string (format: "#[0-9a-f]{6}") to the
   * corresponding RGB values.
   * 
   * The values range from [0-1]. 1 corresponds to all of that color
   * (255) and 0 to no color (0)
   * 
   * @note No verification is being done to the format of the string. The
   * output of this method is undefined for malformatted strings
   * 
   * @param color the hexadecimal string (format: "#[0-9a-f]{6}")
   * 
   * @returns a tuple with 3 floats:
   * 0 -> the red part of the color
   * 1 -> the green part of the color
   * 2 -> the blue part of the color
  */
  static std::tuple<float, float, float> parseHexColor(std::string color);
};
