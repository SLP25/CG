#include <stdlib.h>
#include "glut.hpp"
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

  windowParser.validate_attrs({"width", "height", "axis"});
  windowParser.validate_node({});

  windowSize = windowParser.as_tuple<int, int>({"width", "height"});
  std::string axisStr = "true";
  windowParser.get_opt_attr("axis", axisStr);

  //Convert to lowercase
  for(int i = 0; i < (int)axisStr.size(); i++)
    axisStr[i] = tolower(axisStr[i]);

  if(axisStr== "true") {
    this->axis = true;
  } else if(axisStr == "false") {
    this->axis = false;
  } else {
    this->axis = true;
    std::cout << "Warning: Invalid attribute for axis property of world. Defaulting to true" << std::endl;
  }
}

void World::initScene() {

  // put GLUT's init here
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(std::get<0>(windowSize), std::get<1>(windowSize));
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Model Viewer 3000");

#ifndef __APPLE__
	glewInit();
#endif

  glEnableClientState(GL_VERTEX_ARRAY);

  // some OpenGL settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  Shape::initShapes();
}

void World::changeSize(int width, int height) {
  camera->changeSize({width, height});
}

void World::renderScene() {
  // clear buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera->setupScene();

  if(this->axis)
    drawAxis();
  root.draw();

  // End of frame
  glutSwapBuffers();
  glutPostRedisplay();
}

void World::handleKey(unsigned char key,
                      int x,
                      int y) {
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
