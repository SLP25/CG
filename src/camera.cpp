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

#define ON_READ_ANG 0.1f
#define ON_READ_DIST 0.5f

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

  /* Validating the camera node. */
  parser.validate_node({"position", "lookAt", "up", "projection"});
  parser.validate_max_nodes(1, {"position", "lookAt", "up", "projection"});
  parser.validate_attrs({"type"});

  /* Parsing and validating the position node. */
  XMLParser positionNode = parser.get_node("position");

  positionNode.validate_attrs({"x", "y", "z"});
  positionNode.validate_node({});

  position = positionNode
                       .as_tuple<float, float, float>({"x", "y", "z"});


  /* Parsing and validating the lookAt node. */
  XMLParser lookAtNode = parser.get_node("lookAt");

  lookAtNode.validate_attrs({"x", "y", "z"});
  lookAtNode.validate_node({});

  lookAt = lookAtNode.as_tuple<float, float, float>({"x", "y", "z"});

  /* Parsing and validating the up node. */
  XMLParser upNode = parser.get_node("up");

  upNode.validate_attrs({"x", "y", "z"});
  upNode.validate_node({});

  up = upNode.as_tuple<float, float, float>({"x", "y", "z"});

  /* Parsing and validating the projection node. */
  XMLParser projection = parser.get_node("projection");

  projection.validate_attrs({"fov", "near", "far"});
  projection.validate_node({});

  fov = projection.get_attr<float>("fov");
  near = projection.get_attr<float>("near");
  far = projection.get_attr<float>("far");
}

void PolarCamera::changeSize(WindowSize windowSize) {
  Camera::defaultChangeSize(windowSize, fov, near, far);
}

void PolarCamera::setupScene() {
  glLoadIdentity();
  gluLookAt(std::get<0>(position), std::get<1>(position), std::get<2>(position),
            std::get<0>(lookAt), std::get<1>(lookAt), std::get<2>(lookAt),
            std::get<0>(up), std::get<1>(up), std::get<2>(up));
}

void PolarCamera::handleSpecialKey(int key, __attribute__((unused)) int x,
                                   __attribute__((unused)) int y) {
  Vector aux, perp, trasf;
  
  switch (key) {
  case GLUT_KEY_UP: 
    aux = difference(lookAt, position);
    if (angle(up, aux) - ON_READ_ANG > 0) {
      perp = normalize(crossProduct(up, aux));
      trasf = rotate(perp, aux, -ON_READ_ANG);
      position = addVector(lookAt, trasf);
      glutPostRedisplay();
    }
    break;

  case GLUT_KEY_DOWN:
    aux = difference(lookAt, position);
    if (angle(inverse(up), aux) - ON_READ_ANG > 0) {
      perp = normalize(crossProduct(up, aux));
      trasf = rotate(perp, aux, ON_READ_ANG);
      position = addVector(lookAt, trasf);
      glutPostRedisplay();
    }
    break;

  case GLUT_KEY_LEFT:
    aux = difference(lookAt, position);
    trasf = rotate(up, aux, -ON_READ_ANG);
    position = addVector(lookAt, trasf);
    glutPostRedisplay();
    break;

  case GLUT_KEY_RIGHT:
    aux = difference(lookAt, position);
    trasf = rotate(up, aux, ON_READ_ANG);
    position = addVector(lookAt, trasf);
    glutPostRedisplay();
    break;
  }
}
void PolarCamera::handleKey(unsigned char key, __attribute__((unused)) int x,
                            __attribute__((unused)) int y) {
  Vector aux, transf;
  
  switch (key) {
  case 'w':

    aux = difference(lookAt, position);
    transf = scale(length(aux) - ON_READ_DIST, normalize(aux));
    
    if (dotProduct(aux, transf) > 0) {
      position = addVector(lookAt, transf);
      glutPostRedisplay();
    }

    break;
  case 's':
    aux = difference(lookAt, position);
    transf = scale(length(aux) + ON_READ_DIST, normalize(aux));
    position = addVector(lookAt, transf);
    glutPostRedisplay();
    break;
  }
}




FPSCamera::FPSCamera(Point position, Vector lookAtVector, Vector up, float fov, float near, float far) {
    this->position = position;
    this->lookAtVector = lookAtVector;
    this->up = up;
    this->fov = fov;
    this->near = near;
    this->far = far;
}

FPSCamera::FPSCamera(XMLParser parser) {
    position = parser.get_node("position").as_tuple<float,float,float>({"x","y","z"});
    Point lookAt = parser.get_node("lookAt").as_tuple<float,float,float>({"x","y","z"});
    lookAtVector=difference(position,lookAt);
    up = parser.get_node("up").as_tuple<float,float,float>({"x","y","z"});
    XMLParser projection = parser.get_node("projection");
    fov = projection.get_attr<float>("fov");
    near = projection.get_attr<float>("near");
    far = projection.get_attr<float>("far");
}


void FPSCamera::changeSize(WindowSize windowSize) {
  Camera::defaultChangeSize(windowSize, fov, near, far);
}

void FPSCamera::setupScene() {
    glLoadIdentity();
    Point lookAt = addVector(position,lookAtVector);
    gluLookAt(std::get<0>(position), std::get<1>(position), std::get<2>(position), 
              std::get<0>(lookAt), std::get<1>(lookAt), std::get<2>(lookAt),
              std::get<0>(up), std::get<1>(up), std::get<2>(up));
}

void FPSCamera::handleKey(unsigned char key, __attribute__((unused)) int x, __attribute__((unused)) int y) {
    switch (key) {
        case 'w':
            position = addVector(position, scale(ON_READ_DIST, normalize(projectToPlane(up,lookAtVector))));
            glutPostRedisplay();
            break;
        case 's':
            position = addVector(position, scale(-ON_READ_DIST, normalize(projectToPlane(up,lookAtVector))));
            glutPostRedisplay();
            break;
        case 'a':
            position = addVector(position, scale(ON_READ_DIST, normalize(crossProduct(up,lookAtVector))));
            glutPostRedisplay();
            break;
        case 'd':
            position = addVector(position, scale(-ON_READ_DIST, normalize(crossProduct(up,lookAtVector))));
            glutPostRedisplay();
            break;
        case 'z':
            position = addVector(position,scale(ON_READ_DIST, normalize(up)));
            glutPostRedisplay();
            break;
        case 'x':
            position = addVector(position,scale(-ON_READ_DIST, normalize(up)));
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

void FPSCamera::handleSpecialKey(int key, __attribute__((unused)) int x, __attribute__((unused)) int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if (angle(up,lookAtVector)-ON_READ_ANG>0){
                lookAtVector=rotate(crossProduct(up,lookAtVector),lookAtVector,-ON_READ_ANG);
            }
            glutPostRedisplay();
            break;

        case GLUT_KEY_DOWN:
            if (angle(inverse(up),lookAtVector)-ON_READ_ANG>0){
                lookAtVector=rotate(crossProduct(up,lookAtVector),lookAtVector,ON_READ_ANG);
            }
            glutPostRedisplay();
            break;

        case GLUT_KEY_LEFT:
            lookAtVector=rotate(up,lookAtVector,ON_READ_ANG);
            glutPostRedisplay();
            break;

        case GLUT_KEY_RIGHT:
            lookAtVector=rotate(up,lookAtVector,-ON_READ_ANG);
            glutPostRedisplay();
            break;
    }
}
