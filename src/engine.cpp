#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include "config.hpp"
#include <tuple>

Config config=Config(
  WindowSize(512.0,512.0),
    Camera(
      Position(3.0,2.0,1.0),
      LookAt(0.0,0.0,0.0),
      Up(0.0,1.0,0.0),
      Projection(60.0,1.0,1000.0)
      ),
    Models({ Shape("plane.3d"), Shape("plane.3d") })
  ) ;

void changeSize(int height,int width) {
  WindowSize size = std::get<0>(config);
  height = std::get<0>(size);
  width = std::get<1>(size);
  Camera camera = std::get<1>(config);
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


void renderScene(void) {
  Camera camera = std::get<1>(config);
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
  Models models = std::get<2>(config);

  // clear buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set camera
  glLoadIdentity();
  gluLookAt( position_x, position_y, position_z, 
             lookat_x, lookat_y, lookat_z,
             up_x, up_y, up_z);
  for (Shape &shape : models)
  {
    shape.draw();
  }
  // End of frame
  glutSwapBuffers();
}

int main(int argc, char **argv) {
  WindowSize size = std::get<0>(config);
  int width = std::get<0>(size);
  int height = std::get<1>(size);
  Camera camera = std::get<1>(config);
  Projection projection = std::get<3>(camera);
  float fov = std::get<0>(projection);
  float near = std::get<1>(projection);
  float far = std::get<2>(projection);
  // put GLUT's init here
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  float ratio = width * 1.0f / height;
  gluPerspective(fov, ratio, near, far);
  glutInitWindowSize(width, height);
  glutCreateWindow("CG@DI");

  // put callback registry here
  glutReshapeFunc(changeSize);
  glutIdleFunc(renderScene);
  glutDisplayFunc(renderScene);

  // some OpenGL settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // enter GLUT's main cycle
  glutMainLoop();

  return 1;
}
