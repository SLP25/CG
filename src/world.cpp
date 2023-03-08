#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "utils.hpp"
#include "world.hpp"
#include <iostream>
#include <fstream>

World::World() {};

World::World(WindowSize windowSize, Camera* camera, std::vector<Model> models) {
    this->windowSize = windowSize;
    this->camera = std::unique_ptr<Camera>(camera);
    this->models = models;
}

World::World(XMLParser parser) {

    windowSize = parser.get_node("window").as_tuple<int,int>({"width", "height"});
    camera = Camera::parse(parser.get_node({"camera"}));
    models = std::vector<Model>();

    XMLParser aux = parser.get_node("group").get_node("models");

    for (XMLParser& p : aux.get_nodes("model")) {

        string file_name = p.get_attr<string>("file");

        std::ifstream file(file_name);

        if (!file) {

            std::stringstream exception_message;
            exception_message << "XMLParser@model: The file '" << file_name << "' does not exits.";
            throw InvalidXMLStructure(exception_message.str());
        } 

        models.emplace_back(p.as_object<Shape,string>({"file"}));

    }

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

  for (Model &model : models)
    model.draw();

  // End of frame
  glutSwapBuffers();
}

void World::handleKey(__attribute__((unused)) unsigned char key, __attribute__((unused)) int x, __attribute__((unused)) int y) {
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


