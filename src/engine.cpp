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

void changeSize(int height, int width) { 
  /**
   * @brief function to adapt the scene to the change in size of the window
   * @param height the height of the window
   * @param width the width of the window
   */
  world.changeSize(height, width); 

}

void renderScene() {
  /**
   * @brief Renders the scene into the window
   * 
   */
  world.renderScene(); 
}

void handleKey(unsigned char key, int x, int y) {
  /**
   * @brief handles keyboard events in the window
   * @param key the charecter of the key that needs to be handled
   */
  world.handleKey(key, x, y); 
}

void handleSpecialKey(int key, int x, int y) {
  /**
   * @brief handles keyboard events in the case of special keys, such as the arrow keys, in the window
   * @param key the keycode of the key that needs to be handled
   */
  world.handleSpecialKey(key, x, y);
}

int main(int argc, char **argv) {
  /**
   * @brief Engine program entry point
   *
   * @param argc the number of arguments received
   * @param argv the arguments received
   *
   * @return 0 if execution was successful
   * @return 1 if an error occurred
   */

  if (argc != 2) {
    std::cout << "Error: Invalid argument count, expected one argument but got "
              << argc - 1 << "." << std::endl;
    return 0;
  }
  //help command
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
