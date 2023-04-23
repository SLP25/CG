#include "glut.hpp"

#include <sstream>
#include <string>
#include <cstring>

#include <iostream>

#include "model.hpp"
#include "parser.hpp"

Model::Model(Shape shape, std::tuple<float,float,float> color) :
  shape(std::move(std::make_shared<Shape>(shape))),
  color(color)
{}

Model::Model(XMLParser parser) {
  /**
   * @brief parses the model atribute from the xml parser into a Model object
   * @param parser the model xml data
   */
  parser.validate_attrs({"file", "color"});

  this->shape = Shape::fetchShape(parser.get_attr<std::string>("file"));

  //If no color present, print the shape as being white
  std::string colorHex = "#ffffff";
  parser.get_opt_attr("color", colorHex);
  this->color = parseHexColor(colorHex);
}

void Model::draw() {
  /**
   * @brief Draws the model in the window
   * 
   */
  glPolygonMode(GL_FRONT_AND_BACK, GLU_LINE);
  glColor3f(std::get<0>(this->color), std::get<1>(this->color), std::get<2>(this->color));
  shape->draw();
}
