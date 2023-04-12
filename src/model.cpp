#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <sstream>
#include <string>
#include <cstring>

#include <iostream>

#include "model.hpp"
#include "parser.hpp"

Model::Model(Shape shape) { this->shape = shape; }

Model::Model(XMLParser parser) {
  parser.validate_attrs({"file", "color"});
  this->shape = parser.as_object<Shape, std::string>({"file"});
  //If no color present, print the shape as being white
  std::string colorHex = "#ffffff";
  
  parser.get_opt_attr("color", colorHex);
  this->color = this->parseHexColor(colorHex);
}

void Model::draw() {
  glPolygonMode(GL_FRONT_AND_BACK, GLU_FILL);
  glColor3f(std::get<0>(this->color), std::get<1>(this->color), std::get<2>(this->color));
  shape.draw();
}

std::tuple<float, float, float> Model::parseHexColor(std::string colorHex) {
  unsigned int rgb[3];
  for(int i = 0; i < 3; i++) {
    std::stringstream ss;
    ss << std::hex << colorHex.substr(1 + i * 2,2);
    ss >> rgb[i];
  }

  return {(float)rgb[0] / 255.0f, (float)rgb[1] / 255.0f, (float)rgb[2] / 255.0f};
}
