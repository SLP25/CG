#ifdef __APPLE__
#include <GLUT/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "parser.hpp"
#include "utils.hpp"
#include "world.hpp"

World world;

void changeSize(int height, int width) { world.changeSize(height, width); }

void renderScene() { world.renderScene(); }

void handleKey(unsigned char key, int x, int y) { world.handleKey(key, x, y); }

void handleSpecialKey(int key, int x, int y) {
  world.handleSpecialKey(key, x, y);
}

int main(int argc, char **argv) {

  if (argc != 2) {
    std::cout << "Error: Invalid argument count, expected one argument but got "
              << argc - 1 << "." << std::endl;
    return 0;
  }

  std::string arg = argv[1];
  if (arg == "help") {
    std::cout << "usage: engine [argument]\n";
    std::cout << "Arguments (and their description):\n";
    std::cout << "help    : displays the current message.\n";
    std::cout << "path    : set the configuration file (mandatory argument).\n";
    std::cout << std::endl;
    return 1;
  }

  /* Parse the configuration file. */
  XMLParser parser = XMLParser(arg);
  parser.validate_node({"world"});
  parser.validate_max_nodes(1, {"world"});
  world = World(parser.get_node("world")); /* Set up the world. */

  // put GLUT's init here
  glutInit(&argc, argv);

  world.initScene();

  // glewInit(); after createWindow and before any other glut funcs

  /* Put callback registry here. */
  glutReshapeFunc(changeSize);
  glutDisplayFunc(renderScene);
  glutKeyboardFunc(handleKey);
  glutSpecialFunc(handleSpecialKey);

  /* Enter GLUT's main loop. */
  glutMainLoop();
  return 1;
}
