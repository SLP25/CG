#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "config.hpp"
#include "shapegenerator.hpp"
#include "parser.hpp"

using namespace std;

XMLParser parser = XMLParser("./xml/dummy.xml");
World world = parser.parse();


void changeSize(int height,int width) {
  world.changeSize(height, width);
}

void renderScene() {
  world.renderScene();
}

void handleKey(unsigned char key, int x, int y) {
  world.handleKey(key, x, y);
}

void handleSpecialKey(int key, int x, int y) {
  world.handleSpecialKey(key, x, y);
}

int main(int argc, char **argv) {

    world.initScene(&argc, argv);
    glutCreateWindow("Model Viewer 3000");

    // put callback registry here
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    //glutIdleFunc(renderScene);
    glutKeyboardFunc(handleKey);
    glutSpecialFunc(handleSpecialKey);

    // enter GLUT's main cycle
    glutMainLoop();
    return 1;
}
