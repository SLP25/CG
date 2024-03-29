#include <stdlib.h>
#include "glut.hpp"
#include "utils.hpp"
#include "world.hpp"
#include "texture.hpp"
#include <fstream>
#include <iostream>

World::World() { }

World::World(WindowSize windowSize, Camera *camera, Lighting lighting, Group&& root) :
  srcFile(""),
  windowSize(windowSize),
  camera(std::unique_ptr<Camera>(camera)),
  lighting(lighting),
  root(std::move(root))
{}

World::World(std::string filePath) :
  srcFile(filePath)
{
  XMLParser parser = parseXMLFile(filePath);
  parseWindow(parser);
  parseCamera(parser);
  parseLights(parser);
  parseRootGroup(parser);
}

XMLParser World::parseXMLFile(std::string filePath) {
  /* Parse the configuration file. */
  XMLParser root = XMLParser(filePath);

  root.validate_node({"world"});
  root.validate_max_nodes(1, {"world"});

  XMLParser parser = root.get_node("world");

  /* World node validation. */
  parser.validate_attrs({});
  parser.validate_node({"window", "camera", "lights", "group"});
  parser.validate_max_nodes(1, {"window", "camera", "lights" "group"});

  return parser;
}

void World::parseWindow(XMLParser world) {
  /* Window node validation. */
  XMLParser windowParser = world.get_node("window");

  windowParser.validate_attrs({"width", "height", "axis"});
  windowParser.validate_node({});

  windowSize = windowParser.as_tuple<int, int>({"width", "height"});
  
  std::string axisStr = "true";
  windowParser.get_opt_attr("axis", axisStr);
  this->axis = parseBool(axisStr);
}

void World::parseCamera(XMLParser world) {
  camera = Camera::parse(world.get_node({"camera"}));
}

void World::parseLights(XMLParser world) {
  for (auto n : world.get_nodes("lights"))
    this->lighting = Lighting(n);
}

void World::parseRootGroup(XMLParser world) {
  for (auto n : world.get_nodes("group"))
    this->root = Group(n);
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
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  // some OpenGL settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glEnable(GL_RESCALE_NORMAL);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  lighting.initScene();

  Shape::initShapes();
  Texture::initTextures();
}

void World::changeSize(int width, int height) {
  camera->changeSize({width, height});
}

void World::renderScene() {
  // clear buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera->setupScene();
  lighting.setupScene();

  if(this->axis)
    drawAxis();
  
  int culled = root.draw(camera->viewFrustum());
  glutSetWindowTitle(("Culled Shapes: " + std::to_string(culled)).c_str());

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

  //hot reload (lights, models, shapes and textures)
  if (key == 'r') {
    try {
      Shape::clearCache();
      Texture::clearCache();
      
      XMLParser parser = parseXMLFile(srcFile);
      parseWindow(parser);
      parseLights(parser);
      parseRootGroup(parser);

      lighting.initScene();
      Shape::initShapes();
      Texture::initTextures();
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }

  //reload only camera
  if (key == 'c') {
    try {
      float ratio = camera->ratio;
      XMLParser parser = parseXMLFile(srcFile);
      parseCamera(parser);
      camera->ratio = ratio;
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
  camera->handleKey(key, x, y);
}

void World::handleSpecialKey(int key, int x, int y) {
  camera->handleSpecialKey(key, x, y);
}

void World::drawAxis() {
  float red[] = { 1.0, 0.0, 0.0, 1.0 };
  float green[] = { 0.0, 1.0, 0.0, 1.0 };
  float blue[] = { 0.0, 0.0, 1.0, 1.0 };
  float black[] = { 0.0, 0.0, 0.0, 1.0 };

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
  glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);
  Texture::unbind();

  glBegin(GL_LINES);

  // X-axis in red
  //glColor3f(1.0f, 0.0f, 0.0f);
  glMaterialfv(GL_FRONT, GL_EMISSION, red);
  glVertex3f(-1000.0f, 0.0f, 0.0f);
  glVertex3f(1000.0f, 0.0f, 0.0f);
  // Y Axis in Green
  //glColor3f(0.0f, 1.0f, 0.0f);
  glMaterialfv(GL_FRONT, GL_EMISSION, green);
  glVertex3f(0.0f, -1000.0f, 0.0f);
  glVertex3f(0.0f, 1000.0f, 0.0f);
  // Z Axis in Blue
  //glColor3f(0.0f, 0.0f, 1.0f);
  glMaterialfv(GL_FRONT, GL_EMISSION, blue);
  glVertex3f(0.0f, 0.0f, -1000.0f);
  glVertex3f(0.0f, 0.0f, 1000.0f);

  glEnd();
}
