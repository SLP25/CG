#ifdef __APPLE__
#include <GLUT/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "utils.hpp"
#include "parser.hpp"
#include "transformation.hpp"

std::unique_ptr<Transformation> Transformation::parse(XMLParser parser) {
  return dynamicParser<Transformation,Translation,Rotation,Scale>::parse(parser.name(), parser);
}

Translation::Translation(XMLParser parser) {
  parser.validate_attrs({"x", "y", "z"});

  this->x = parser.get_attr<float>("x");
  this->y = parser.get_attr<float>("y");
  this->z = parser.get_attr<float>("z");
}

Transformation* Translation::clone() {
  return new Translation(*this);
}

void Translation::apply() { glTranslatef(this->x, this->y, this->z); }

Rotation::Rotation(XMLParser parser) {
  parser.validate_attrs({"x", "y", "z", "angle"});

  this->x = parser.get_attr<float>("x");
  this->y = parser.get_attr<float>("y");
  this->z = parser.get_attr<float>("z");
  this->angle = parser.get_attr<float>("angle");
}

Transformation* Rotation::clone() {
  return new Rotation(*this);
}

void Rotation::apply() { glRotatef(this->angle, this->x, this->y, this->z); }

Transformation::~Transformation() {}

Scale::Scale(XMLParser parser) {

  float s;
  if (parser.get_opt_attr<float>("s", s))
  {
    parser.validate_attrs({"s"});
    this->x = this->y = this->z = s;
  }
  else
  {
    parser.validate_attrs({"x", "y", "z"});
    this->x = parser.get_attr<float>("x");
    this->y = parser.get_attr<float>("y");
    this->z = parser.get_attr<float>("z");
  }
}

Transformation* Scale::clone() {
  return new Scale(*this);
}

void Scale::apply() { glScalef(this->x, this->y, this->z); }
