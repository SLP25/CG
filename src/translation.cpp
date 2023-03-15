#ifdef __APPLE__
#include <GLUT/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "parser.hpp"
#include "transformation.hpp"

std::unique_ptr<Transformation> Transformation::parse(XMLParser parser) {
  // TODO: BACE REFACTOR THIS
  if (parser.name() == "translate") {
    return std::make_unique<Translation>(parser);
  } else if (parser.name() == "rotate") {
    return std::make_unique<Rotation>(parser);
  } else if (parser.name() == "scale") {
    return std::make_unique<Scale>(parser);
  } else {
    throw InvalidXMLStructure("Invalid transformation with name" +
                              parser.name() + ".");
  }
}

Translation::Translation(XMLParser parser) {
  parser.validate_attrs({"x", "y", "z"});

  this->x = parser.get_attr<float>("x");
  this->y = parser.get_attr<float>("y");
  this->z = parser.get_attr<float>("z");
}

void Translation::apply() { glTranslatef(this->x, this->y, this->z); }

Rotation::Rotation(XMLParser parser) {
  parser.validate_attrs({"x", "y", "z", "angle"});

  this->x = parser.get_attr<float>("x");
  this->y = parser.get_attr<float>("y");
  this->z = parser.get_attr<float>("z");
  this->angle = parser.get_attr<float>("angle");
}

void Rotation::apply() { glRotatef(this->angle, this->x, this->y, this->z); }

Transformation::~Transformation() {}

Scale::Scale(XMLParser parser) {
  parser.validate_attrs({"x", "y", "z"});

  this->x = parser.get_attr<float>("x");
  this->y = parser.get_attr<float>("y");
  this->z = parser.get_attr<float>("z");
}

void Scale::apply() { glScalef(this->x, this->y, this->z); }
