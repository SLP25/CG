#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include "config.hpp"
#include "utils.hpp"

World::World(WindowSize windowSize, Camera camera, Models models) {
    this->windowSize = windowSize;
    this->camera = camera;
    this->models = models;
}

World::World() {};

void World::initScene(int* argc, char** argv) {

    // put GLUT's init here
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);

    int width = std::get<0>(windowSize);
    int height = std::get<1>(windowSize);

    Projection projection = std::get<3>(camera);
    float fov = std::get<0>(projection);
    float near = std::get<1>(projection);
    float far = std::get<2>(projection);

    float ratio = width * 1.0f / height;

    gluPerspective(fov, ratio, near, far);
    glutInitWindowSize(width, height);

    // some OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void World::changeSize(int width, int height) {
    // Prevent a divide by zero, when window is too short
	// (you can't make a window with zero width).
	if (height == 0)
		height = 1;
    
    Projection projection = std::get<3>(camera);
    float fov = std::get<0>(projection);
    float near = std::get<1>(projection);
    float far = std::get<2>(projection);

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

void World::renderScene() {
  Position position = std::get<0>(camera);
  float position_x = std::get<0>(position);
  float position_y = std::get<1>(position);
  float position_z = std::get<2>(position);
  LookAt lookat = std::get<1>(camera);
  float lookat_x = std::get<0>(lookat);
  float lookat_y = std::get<1>(lookat);
  float lookat_z = std::get<2>(lookat);
  Up up = std::get<2>(camera);
  float up_x = std::get<0>(up);
  float up_y = std::get<1>(up);
  float up_z = std::get<2>(up);

  // clear buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set camera
  glLoadIdentity();
  gluLookAt( position_x, position_y, position_z, 
             lookat_x, lookat_y, lookat_z,
             up_x, up_y, up_z);

  drawAxis();

  for (Shape &shape : models)
    shape.draw();

  // End of frame
  glutSwapBuffers();
}

void World::handleKey(__attribute__((unused)) unsigned char key, __attribute__((unused)) int x, __attribute__((unused)) int y) {
}

void World::handleSpecialKey(int key, __attribute__((unused)) int x, __attribute__((unused)) int y) {

    Point& pos = std::get<0>(camera);

    float d = sqrt(std::get<0>(pos) * std::get<0>(pos) + std::get<2>(pos) * std::get<2>(pos));
    float a = atan(std::get<2>(pos) / std::get<0>(pos));
    
    switch (key) {
        case GLUT_KEY_UP:
            std::get<0>(camera) = addVector(std::get<0>(camera), {0, 0.1, 0});
            glutPostRedisplay();
            break;

        case GLUT_KEY_DOWN:
            std::get<0>(camera) = addVector(std::get<0>(camera), {0, -0.1, 0});
            glutPostRedisplay();
            break;

        case GLUT_KEY_LEFT:
            a -= 0.1;
            pos = { d * cos(a), std::get<1>(pos), d * sin(a) };
            glutPostRedisplay();
            break;

        case GLUT_KEY_RIGHT:
            a += 0.1;
            pos = { d * cos(a), std::get<1>(pos), d * sin(a) };
            glutPostRedisplay();
            break;
    }
}

void World::drawAxis() {
    glBegin(GL_LINES);

	// X-axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);

	glEnd();
}


