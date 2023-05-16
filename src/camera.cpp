#include "glut.hpp"

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
  /**
   * @brief parses the camera atribute from the xml parser into a Camera object
   * 
   * @param parser the camera xml data
   */

  std::string type;

  if (!parser.get_opt_attr("type", type)) // Verify the camera type (Polar) if the type isn't specified
    return std::make_unique<PolarCamera>(parser);
  return dynamicParser<Camera,PolarCamera,FPSCamera>::parse(parser);
}

Camera::~Camera() {}

void Camera::handleKey(unsigned char key,
                       int x,
                       int y) {}

void Camera::handleSpecialKey(int key,
                              int x,
                              int y) {}

void Camera::defaultChangeSize(WindowSize windowSize, float fov, float near,
                               float far) {
  /**
   * @brief ajustes the camera to the size of the window
   * 
   * @param windowSize The size of the window
   * @param fov the field of view of the camera
   * @param near the minimum distance from an object to the camera for it to be drawn
   * @param far the maximum distance from an object to the camera for it to be drawn
   */
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


bool PolarCamera::accepts(XMLParser parser) {
  return parser.get_attr<std::string>("type") == "polar";
}

PolarCamera::PolarCamera(XMLParser parser) {
  /**
   * @brief parses the camera atribute from the xml parser into a PolarCamera object
   * @param parser the camera xml data
   */

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
  /**
   * @brief Ajustes the camera to the size of the window
   * @param windowSize The size of the window
   */
  Camera::defaultChangeSize(windowSize, fov, near, far);
}

void PolarCamera::setupScene() {
  /**
   * @brief loads the camera into the world
   * 
   */
  glLoadIdentity();
  gluLookAt(GET_ALL(position), GET_ALL(lookAt), GET_ALL(up));
}

void PolarCamera::handleSpecialKey(int key, int x,
                                   int y) {
  /**
   * @brief handles special keys events like the arrow keys for rotational movement of the camera
   * @param key the keycode of the special key that was pressed
   */
  Vector aux, perp, trasf;
  
  switch (key) {
  case GLUT_KEY_UP: //look down
    aux = position - lookAt;
    if (angle(up, aux) - ON_READ_ANG > 0) { //don't allow to look past the directly down plane
      perp = normalize(up ^ aux);
      trasf = rotate(perp, aux, -ON_READ_ANG);
      position = lookAt + trasf;
      //glutPostRedisplay();
    }
    break;

  case GLUT_KEY_DOWN://look up
    aux = position - lookAt;
    if (angle(-up, aux) - ON_READ_ANG > 0) {//don't allow to look past the directly up plane
      perp = normalize(up ^ aux);
      trasf = rotate(perp, aux, ON_READ_ANG);
      position = lookAt + trasf;
      //glutPostRedisplay();
    }
    break;

  case GLUT_KEY_LEFT: // rotate to the left
    aux = position - lookAt;
    trasf = rotate(up, aux, -ON_READ_ANG);
    position = lookAt + trasf;
    //glutPostRedisplay();
    break;

  case GLUT_KEY_RIGHT: // rotate to the right
    aux = position - lookAt;
    trasf = rotate(up, aux, ON_READ_ANG);
    position = lookAt + trasf;
    //glutPostRedisplay();
    break;
  }
}
void PolarCamera::handleKey(unsigned char key, int x,
                            int y) {
  
   /**
   * @brief handles keys events for distance from the look_at point
   * @param key the character of the key that was pressed
   */
  Vector aux, transf;
  
  
  switch (key) {
  case 'w': //zoom in

    aux = position - lookAt;
    transf = (length(aux) - ON_READ_DIST) * normalize(aux);
    
    if (aux * transf > 0) { //don't let get to close
      position = lookAt + transf;
      //glutPostRedisplay();
    }

    break;
  case 's': //zoom out
    aux = position - lookAt;
    transf = (length(aux) + ON_READ_DIST) * normalize(aux);
    position = lookAt + transf;
    //glutPostRedisplay();
    break;
  }
}


bool FPSCamera::accepts(XMLParser parser) {
  return parser.get_attr<std::string>("type") == "fps";
}

FPSCamera::FPSCamera(Point position, Vector lookAtVector, Vector up, float fov, float near, float far) {
  /**
   * @brief create a FPSCamera object from its components
   * @param The position of the camera in space
   * @param lookAtVector To where the camera is looking at
   * @param up The vector indicating the up direction
   * @param fov the field of view of the camera
   * @param near the minimum distance between an object and the camera for it to be drawn
   * @param far the maximum distance between an object and the camera for it to be drawn
   */
    this->position = position;
    this->lookAtVector = lookAtVector;
    this->up = up;
    this->fov = fov;
    this->near = near;
    this->far = far;
}

FPSCamera::FPSCamera(XMLParser parser) {
  /**
   * @brief parses the camera atribute from the xml parser into a FPSCamera object
   * @param parser the camera xml data
   */
    position = parser.get_node("position").as_tuple<float,float,float>({"x","y","z"});
    Point lookAt = parser.get_node("lookAt").as_tuple<float,float,float>({"x","y","z"});
    lookAtVector=lookAt - position;
    up = parser.get_node("up").as_tuple<float,float,float>({"x","y","z"});
    XMLParser projection = parser.get_node("projection");
    fov = projection.get_attr<float>("fov");
    near = projection.get_attr<float>("near");
    far = projection.get_attr<float>("far");
}


void FPSCamera::changeSize(WindowSize windowSize) {
  /**
   * @brief ajustes the camera to the size of the window
   * @param windowSize The size of the window
   */
  Camera::defaultChangeSize(windowSize, fov, near, far);
}

void FPSCamera::setupScene() {
  /**
   * @brief loads the camera into the world
   * 
   */
  glLoadIdentity();
  Point lookAt = position + lookAtVector;
  gluLookAt(GET_ALL(position), GET_ALL(lookAt), GET_ALL(up));
}

void FPSCamera::handleKey(unsigned char key, int x, int y) {
    /**
     * @brief handles keys events for movement
     * @param key the character of the key that was pressed
     */
	switch (key) {
        case 'w'://move forwards
            position = position + ON_READ_DIST * normalize(projectToPlane(up,lookAtVector));
            //glutPostRedisplay();
            break;
        case 's'://move backwards
            position = position - ON_READ_DIST * normalize(projectToPlane(up,lookAtVector));
            //glutPostRedisplay();
            break;
        case 'a'://move to the left
            position = position + ON_READ_DIST * normalize(up ^ lookAtVector);
            //glutPostRedisplay();
            break;
        case 'd'://move to the right
            position = position - ON_READ_DIST * normalize(up ^ lookAtVector);
            //glutPostRedisplay();
            break;
        case 'z'://move upwards
            position = position + ON_READ_DIST * normalize(up);
            //glutPostRedisplay();
            break;
        case 'x'://move downwards
            position = position - ON_READ_DIST * normalize(up);
            //glutPostRedisplay();
            break;
        default:
            break;
    }
}

void FPSCamera::handleSpecialKey(int key, int x, int y) {
    /**
     * @brief handles special keys events like the arrow keys for rotational movement of the camera
     * @param key the keycode of the special key that was pressed
     */
	switch (key) {
        case GLUT_KEY_UP://rotate camera upwards
            if (angle(up, lookAtVector) - ON_READ_ANG > 0) {//limit so that the camera doesn't go past directly up
                lookAtVector = rotate(up ^ lookAtVector, lookAtVector, -ON_READ_ANG);
            }
            //glutPostRedisplay();
            break;

        case GLUT_KEY_DOWN://rotate camera downwards
            if (angle(-up, lookAtVector) - ON_READ_ANG > 0) {//limit so that the camera doesn't go past directly down
                lookAtVector = rotate(up ^ lookAtVector, lookAtVector, ON_READ_ANG);
            }
            //glutPostRedisplay();
            break;

        case GLUT_KEY_LEFT://rotate camera to the left
            lookAtVector=rotate(up,lookAtVector,ON_READ_ANG);
            //glutPostRedisplay();
            break;

        case GLUT_KEY_RIGHT://rotate camera to the right
            lookAtVector=rotate(up,lookAtVector,-ON_READ_ANG);
            //glutPostRedisplay();
            break;
    }
}
