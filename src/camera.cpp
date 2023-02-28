#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "config.hpp"
#include "utils.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera() { }

Camera::Camera(Point position, Point lookAt, Vector up, float fov, float near, float far) {
    this->position = position;
    this->lookAt = lookAt;
    this->up = up;
    this->fov = fov;
    this->near = near;
    this->far = far;
}

Camera::Camera(XMLParser parser) {
    position = parser.get_node("position").as_tuple<float,float,float>({"x","y","z"});
    lookAt = parser.get_node("lookAt").as_tuple<float,float,float>({"x","y","z"});
    up = parser.get_node("up").as_tuple<float,float,float>({"x","y","z"});

    XMLParser projection = parser.get_node("projection");
    fov = projection.get_attr<float>("fov");
    near = projection.get_attr<float>("near");
    far = projection.get_attr<float>("far");
}

void Camera::initScene(WindowSize windowSize) {
    int width = std::get<0>(windowSize);
    int height = std::get<1>(windowSize);

    float ratio = width * 1.0f / height;

    gluPerspective(fov, ratio, near, far);
    glutInitWindowSize(width, height);
}

void Camera::changeSize(int width, int height) {
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

void Camera::setupScene() {
    // set camera
    glLoadIdentity();
    gluLookAt(std::get<0>(position), std::get<1>(position), std::get<2>(position), 
              std::get<0>(lookAt), std::get<1>(lookAt), std::get<2>(lookAt),
              std::get<0>(up), std::get<1>(up), std::get<2>(up));
}

void Camera::handleKey(__attribute__((unused)) unsigned char key, __attribute__((unused)) int x, __attribute__((unused)) int y) {
    switch (key) {
        case 'w':
            position = addVector(position, {0, 0, -1});
            lookAt = addVector(lookAt,{0, 0, -1});
            glutPostRedisplay();
            break;
        case 's':
            position = addVector(position, {0, 0, 1});
            lookAt = addVector(lookAt,{0, 0, 1});
            glutPostRedisplay();
            break;
        case 'a':
            position = addVector(position, {-1, 0, 0});
            lookAt = addVector(lookAt,{-1, 0, 0});
            glutPostRedisplay();
            break;
        case 'd':
            position = addVector(position, {1, 0, 0});
            lookAt = addVector(lookAt,{1, 0, 0});
            glutPostRedisplay();
            break;
    }
}

void Camera::handleSpecialKey(int key, __attribute__((unused)) int x, __attribute__((unused)) int y) {
    float a = 0.1;

    switch (key) {
        case GLUT_KEY_UP:
            position = addVector(position, {0, 0.1, 0});
            glutPostRedisplay();
            break;

        case GLUT_KEY_DOWN:
            position = addVector(position, {0, -0.1, 0});
            glutPostRedisplay();
            break;

        case GLUT_KEY_LEFT:
            position = { cos(a) * std::get<0>(position) - sin(a) * std::get<2>(position),
                         std::get<1>(position),
                         cos(a) * std::get<2>(position) + sin(a) * std::get<0>(position) };
            glutPostRedisplay();
            break;

        case GLUT_KEY_RIGHT:
            position = { cos(-a) * std::get<0>(position) - sin(-a) * std::get<2>(position),
                         std::get<1>(position),
                         cos(-a) * std::get<2>(position) + sin(-a) * std::get<0>(position) };
            glutPostRedisplay();
            break;
    }
}