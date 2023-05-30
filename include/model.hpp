#pragma once

/**
 * @file model.hpp
 * @brief File declaring the #Model class, used to represent the models in the scene
*/

#include "parser.hpp"
#include "shape.hpp"
#include "texture.hpp"

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
  std::shared_ptr<Texture> texture;

  /**
   * @brief The colors of the model in RGB.
   * 
   * The values range from [0-1]. 1 corresponds to all of that color
   * (255) and 0 to no color (0).
   * 
   * 0 -> the red part of the color
   * 1 -> the green part of the color
   * 2 -> the blue part of the color
  */
  Color emission;
  Color ambient;
  Color diffuse;
  Color specular;
  float shininess;

  void readColor(XMLParser color);

public:
  /**
   * @brief Constructs a new Model object from a given Shape, Texture and Colors
   *
   * @param shape The given shape
   */
  Model(Shape shape, Texture texture, Color emission, Color ambient, Color diffuse, Color specular, float shininess);
  Model(XMLParser parser);

  /**
   * @brief Draws the model by calling glut's static functions
   *
   */
  int draw(const Frustum& viewFrustum);
};
