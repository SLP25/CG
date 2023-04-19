#include "glut.hpp"

#include "utils.hpp"
#include "parser.hpp"
#include "transformation.hpp"
#include <iostream>

std::unique_ptr<Transformation> Transformation::parse(XMLParser parser) {
  return dynamicParser<Transformation,Translation,Rotation,Scale>::parse(parser.name(), parser);
}

Translation::Translation(XMLParser parser) {
  parser.validate_node({});
  parser.validate_attrs({"x", "y", "z"});

  this->x = parser.get_attr<float>("x");
  this->y = parser.get_attr<float>("y");
  this->z = parser.get_attr<float>("z");
}

Transformation* Translation::clone() {
  return new Translation(*this);
}

void Translation::apply() { glTranslatef(this->x, this->y, this->z); }

bool nonLazyAND(bool a, bool b) { return a && b; }

Rotation::Rotation(XMLParser parser) : time(0.0f), angle(0.0f) {

  parser.validate_node({});
  parser.validate_attrs({"x", "y", "z", "angle", "time"});

  this->x = parser.get_attr<float>("x");
  this->y = parser.get_attr<float>("y");
  this->z = parser.get_attr<float>("z");
  if (nonLazyAND(!parser.get_opt_attr<float>("angle", this->angle),
                 !parser.get_opt_attr<float>("time", this->time)))
    throw InvalidXMLStructure("Rotate must define either angle or time (or both)");
}

Transformation* Rotation::clone() {
  return new Rotation(*this);
}

void Rotation::apply() {
  float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
  float w = this->time != 0 ? 360 / this->time : 0;

  glRotatef(this->angle + t * w, this->x, this->y, this->z);
}

Transformation::~Transformation() {}

Scale::Scale(XMLParser parser) {

  parser.validate_node({});
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
