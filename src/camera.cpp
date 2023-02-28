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

Camera::Camera(Point position, float angleXZ,float angleZY, Vector up, float fov, float near, float far) {
    this->position = position;
    this->angleXZ = angleXZ;
    this->angleZY = angleZY;
    this->up = up;
    this->fov = fov;
    this->near = near;
    this->far = far;
}

Camera::Camera(XMLParser parser) {
    position = parser.get_node("position").as_tuple<float,float,float>({"x","y","z"});
    Point lookAt = parser.get_node("lookAt").as_tuple<float,float,float>({"x","y","z"});
    Vector lookAtVector=difference(lookAt,position);
    angleXZ = angle({0,0,1},{std::get<0>(lookAtVector),0,std::get<2>(lookAtVector)});
    angleZY = (PI/4) - angle({0,1,0},{0,std::get<1>(lookAtVector),std::get<2>(lookAtVector)});
    up = parser.get_node("up").as_tuple<float,float,float>({"x","y","z"});

    XMLParser projection = parser.get_node("projection");
    fov = projection.get_attr<float>("fov");
    near = projection.get_attr<float>("near");
    far = projection.get_attr<float>("far");
}

Vector Camera::getLookAtVector(){
    return addVector(position,{cos(angleXZ),sin(angleZY),sin(angleXZ)});
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
    Point lookAt=getLookAtVector();
    gluLookAt(std::get<0>(position), std::get<1>(position), std::get<2>(position), 
              std::get<0>(lookAt), std::get<1>(lookAt), std::get<2>(lookAt),
              std::get<0>(up), std::get<1>(up), std::get<2>(up));
}

void Camera::handleKey(__attribute__((unused)) unsigned char key, __attribute__((unused)) int x, __attribute__((unused)) int y) {
    switch (key) {
        case 'w':
            position = addVector(position, {0, 0, -1});
            glutPostRedisplay();
            break;
        case 's':
            position = addVector(position, {0, 0, 1});
            glutPostRedisplay();
            break;
        case 'a':
            position = addVector(position, {-1, 0, 0});
            glutPostRedisplay();
            break;
        case 'd':
            position = addVector(position, {1, 0, 0});
            glutPostRedisplay();
            break;
        case 'z':
            position = addVector(position,{0,0.1,0});
            glutPostRedisplay();
            break;
        case 'x':
            position = addVector(position,{0,-0.1,0});
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

void Camera::handleSpecialKey(int key, __attribute__((unused)) int x, __attribute__((unused)) int y) {
    switch (key) {
        case GLUT_KEY_DOWN:
            if (angleZY>-(PI/2))
                angleZY-=ONERAD;
            glutPostRedisplay();
            break;

        case GLUT_KEY_UP:
            if (angleZY<(PI/2))
                angleZY+=ONERAD;
            glutPostRedisplay();
            break;

        case GLUT_KEY_LEFT:
            angleXZ-=ONERAD;
            glutPostRedisplay();
            break;

        case GLUT_KEY_RIGHT:
            angleXZ+=ONERAD;
            glutPostRedisplay();
            break;
    }
}