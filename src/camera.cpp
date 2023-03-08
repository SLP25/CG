#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "camera.hpp"
#include "utils.hpp"
#include "world.hpp"

#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <memory>

std::unique_ptr<Camera> Camera::parse(XMLParser parser) {

  std::string type;

  if (!parser.get_opt_attr("type", type))
    return std::make_unique<PolarCamera>(parser);
  return Camera::parseCamera<PolarCamera, FPSCamera>(type, parser);
}

Camera::~Camera() {}

void Camera::handleKey(__attribute__((unused)) unsigned char key,
                       __attribute__((unused)) int x,
                       __attribute__((unused)) int y) {}

void Camera::handleSpecialKey(__attribute__((unused)) int key,
                              __attribute__((unused)) int x,
                              __attribute__((unused)) int y) {}

void Camera::defaultChangeSize(WindowSize windowSize, float fov, float near,
                               float far) {
  int width = std::get<0>(windowSize);
  int height = std::get<1>(windowSize);

  // Prevent a divide by zero, when window is too short
  // (you can't make a window with zero width).
  if (height == 0)
    height = 1;

  // compute window's aspect ratio
  float ratio = width * 1.0f / height;
  // Set the projection matrix as current
  glMatrixMode(GL_PROJECTION);
  // Load the identity matrix
  glLoadIdentity();
  // Set the viewport to be the entire window
  glViewport(0, 0, width, height);
  // Set the perspective
  gluPerspective(fov, ratio, near, far);
  // return to the model view matrix mode
  glMatrixMode(GL_MODELVIEW);
}

PolarCamera::PolarCamera(XMLParser parser) {

  parser.validate_node({"position", "lookAt", "up", "projection"});
  parser.validate_max_nodes(1, {"position", "lookAt", "up", "projection"});
  parser.validate_attrs({});

  Point position = parser.get_node("position")
                       .as_tuple<float, float, float>({"x", "y", "z"});
  lookAt =
      parser.get_node("lookAt").as_tuple<float, float, float>({"x", "y", "z"});
  Vector lookAtVector = difference(position, lookAt);
  radius = length(lookAtVector);
  angleXZ = angle({0, 0, 1},
                  {std::get<0>(lookAtVector), 0, std::get<2>(lookAtVector)});
  angleZY = (M_PI / 2) - angle({0, 1, 0}, {0, std::get<1>(lookAtVector),
                                           std::get<2>(lookAtVector)});
  up = parser.get_node("up").as_tuple<float, float, float>({"x", "y", "z"});
  XMLParser projection = parser.get_node("projection");
  fov = projection.get_attr<float>("fov");
  near = projection.get_attr<float>("near");
  far = projection.get_attr<float>("far");
}

void PolarCamera::changeSize(WindowSize windowSize) {
  Camera::defaultChangeSize(windowSize, fov, near, far);
}
Vector PolarCamera::getPositionVector() {
  return {-cos(angleXZ) * radius, -sin(angleZY) * radius,
          -sin(angleXZ) * radius};
}

void PolarCamera::setupScene() {
  glLoadIdentity();
  Point position = getPositionVector();
  gluLookAt(std::get<0>(position), std::get<1>(position), std::get<2>(position),
            std::get<0>(lookAt), std::get<1>(lookAt), std::get<2>(lookAt),
            std::get<0>(up), std::get<1>(up), std::get<2>(up));
}

void PolarCamera::handleSpecialKey(int key, __attribute__((unused)) int x,
                                   __attribute__((unused)) int y) {
  switch (key) {
  case GLUT_KEY_UP:
    if (angleZY > -M_PI / 2)
      angleZY -= ONERAD;
    glutPostRedisplay();
    break;

  case GLUT_KEY_DOWN:
    if (angleZY < M_PI / 2)
      angleZY += ONERAD;
    glutPostRedisplay();
    break;

  case GLUT_KEY_LEFT:
    angleXZ += ONERAD;
    glutPostRedisplay();
    break;

  case GLUT_KEY_RIGHT:
    angleXZ -= ONERAD;
    glutPostRedisplay();
    break;
  }
}
void PolarCamera::handleKey(unsigned char key, __attribute__((unused)) int x,
                            __attribute__((unused)) int y) {
  switch (key) {
  case 'w':
    if (radius > 0) {
      radius -= 1;
      glutPostRedisplay();
    }
    break;
  case 's':
    radius += 1;
    glutPostRedisplay();
    break;
  }
}

FPSCamera::FPSCamera(Point position, float angleXZ, float angleZY, Vector up,
                     float fov, float near, float far) {
  this->position = position;
  this->angleXZ = angleXZ;
  this->angleZY = angleZY;
  this->up = up;
  this->fov = fov;
  this->near = near;
  this->far = far;
}

FPSCamera::FPSCamera(XMLParser parser) {

  parser.validate_node({"position", "lookAt", "up", "projection"});
  parser.validate_max_nodes(1, {"position", "lookAt", "up", "projection"});
  parser.validate_attrs({});

  position = parser.get_node("position")
                 .as_tuple<float, float, float>({"x", "y", "z"});
  Point lookAt =
      parser.get_node("lookAt").as_tuple<float, float, float>({"x", "y", "z"});
  Vector lookAtVector = difference(position, lookAt);
  angleXZ = angle({1, 0, 0},
                  {std::get<0>(lookAtVector), 0, std::get<2>(lookAtVector)});
  angleZY = 2 * M_PI - angle({0, 1, 0}, {0, std::get<1>(lookAtVector),
                                         std::get<2>(lookAtVector)});
  up = parser.get_node("up").as_tuple<float, float, float>({"x", "y", "z"});
  XMLParser projection = parser.get_node("projection");
  fov = projection.get_attr<float>("fov");
  near = projection.get_attr<float>("near");
  far = projection.get_attr<float>("far");
}

Vector FPSCamera::getLookAtVector() {
  return {cos(angleXZ), sin(angleZY), -sin(angleXZ)};
}

void FPSCamera::changeSize(WindowSize windowSize) {
  Camera::defaultChangeSize(windowSize, fov, near, far);
}

void FPSCamera::setupScene() {
  glLoadIdentity();
  Point lookAt = addVector(position, getLookAtVector());
  gluLookAt(std::get<0>(position), std::get<1>(position), std::get<2>(position),
            std::get<0>(lookAt), std::get<1>(lookAt), std::get<2>(lookAt),
            std::get<0>(up), std::get<1>(up), std::get<2>(up));
}

void FPSCamera::handleKey(unsigned char key, __attribute__((unused)) int x,
                          __attribute__((unused)) int y) {
  Vector fowardsVector = normalize(getLookAtVector());
  switch (key) {
  case 'w':
    position = addVector(
        position, {std::get<0>(fowardsVector), 0, std::get<2>(fowardsVector)});
    glutPostRedisplay();
    break;
  case 's':
    position = addVector(position, inverse({std::get<0>(fowardsVector), 0,
                                            std::get<2>(fowardsVector)}));
    glutPostRedisplay();
    break;
  case 'a':
    position =
        addVector(position, RotateAroundYAxis({std::get<0>(fowardsVector), 0,
                                               std::get<2>(fowardsVector)},
                                              -M_PI / 2));
    glutPostRedisplay();
    break;
  case 'd':
    position =
        addVector(position, RotateAroundYAxis({std::get<0>(fowardsVector), 0,
                                               std::get<2>(fowardsVector)},
                                              M_PI / 2));
    glutPostRedisplay();
    break;
  case 'z':
    position = addVector(position, {0, 0.1, 0});
    glutPostRedisplay();
    break;
  case 'x':
    position = addVector(position, {0, -0.1, 0});
    glutPostRedisplay();
    break;
  default:
    break;
  }
}

void FPSCamera::handleSpecialKey(int key, __attribute__((unused)) int x,
                                 __attribute__((unused)) int y) {
  switch (key) {
  case GLUT_KEY_UP:
    if (angleZY > M_PI / 2)
      angleZY -= ONERAD;
    glutPostRedisplay();
    break;

  case GLUT_KEY_DOWN:
    if (angleZY < (M_PI + M_PI / 2))
      angleZY += ONERAD;
    glutPostRedisplay();
    break;

  case GLUT_KEY_LEFT:
    angleXZ += ONERAD;
    glutPostRedisplay();
    break;

  case GLUT_KEY_RIGHT:
    angleXZ -= ONERAD;
    glutPostRedisplay();
    break;
  }
}
