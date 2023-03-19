#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "model.hpp"
#include "parser.hpp"

Model::Model(Shape shape) { this->shape = shape; }

Model::Model(XMLParser parser) {
  parser.validate_attrs({"file"});
  this->shape = parser.as_object<Shape, std::string>({"file"});
}

void Model::draw() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor3f(1, 1, 1);
  shape.draw();
}
