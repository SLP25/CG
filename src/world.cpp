#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "utils.hpp"
#include "world.hpp"
#include <fstream>
#include <iostream>

World::World() { }

World::World(WindowSize windowSize, Camera *camera, Group&& root) :
  windowSize(windowSize),
  camera(std::unique_ptr<Camera>(camera)),
  root(std::move(root))
{}

World::World(XMLParser parser) :
  camera(Camera::parse(parser.get_node({"camera"}))),
  root(parser.get_node("group"))
{

  /* World node validation. */
  parser.validate_attrs({});
  parser.validate_node({"window", "camera", "group"});
  parser.validate_max_nodes(1, {"window", "camera", "group"});

  /* Window node validation. */
  XMLParser windowParser = parser.get_node("window");

  windowParser.validate_attrs({"width", "height"});
  windowParser.validate_node({});

  windowSize = windowParser.as_tuple<int, int>({"width", "height"});
}

void World::initScene() {

  // put GLUT's init here
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(std::get<0>(windowSize), std::get<1>(windowSize));

  glutInitWindowPosition(100, 100);
  glutCreateWindow("Model Viewer 3000");

  // some OpenGL settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void World::changeSize(int width, int height) {
  camera->changeSize({width, height});
}

void World::renderScene() {
  // clear buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera->setupScene();

  drawAxis();
  root.draw();

  // End of frame
  glutSwapBuffers();
}

void World::handleKey(__attribute__((unused)) unsigned char key,
                      __attribute__((unused)) int x,
                      __attribute__((unused)) int y) {
  // Terminate the engine if 'q' is pressed
  if (key == 'q')
    exit(0); // TODO: More gracious shutdown
  camera->handleKey(key, x, y);
}

void World::handleSpecialKey(int key, int x, int y) {
  camera->handleSpecialKey(key, x, y);
}

void World::drawAxis() {
  glBegin(GL_LINES);

  // X-axis in red
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(-1000.0f, 0.0f, 0.0f);
  glVertex3f(1000.0f, 0.0f, 0.0f);
  // Y Axis in Green
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(0.0f, -1000.0f, 0.0f);
  glVertex3f(0.0f, 1000.0f, 0.0f);
  // Z Axis in Blue
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0.0f, 0.0f, -1000.0f);
  glVertex3f(0.0f, 0.0f, 1000.0f);

  glEnd();
}
