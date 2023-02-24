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
#include <tuple>

World world(
  WindowSize(512.0,512.0),
  Camera(
    Position(3.0,2.0,1.0),
    LookAt(0.0,0.0,0.0),
    Up(0.0,1.0,0.0),
    Projection(60.0,1.0,1000.0)
  ),
Models( {Shape("plane.3d"), Shape("box.3d") })
);

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
  glutCreateWindow("CG@DI");

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
