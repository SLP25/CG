#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "model.hpp"

Model::Model(Shape shape) {
    this->shape = shape;
}

void Model::draw() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(1, 1, 1);
    shape.draw();
}