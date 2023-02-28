#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <iostream>

#endif

#include "config.hpp"
#include "parser.hpp"

using namespace std;

World world;

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

    if (argc != 2) {
        cout << "Error: Invalid argument count, expected one argument but got " << argc - 1 << "." << endl;
        return 0;
    }

    string arg = argv[1];
    if (arg == "help") {
        cout << "usage: engine [argument]\n";
        cout << "Arguments (and their description):\n";
        cout << "help    : displays the current message.\n";
        cout << "path    : set the configuration file (mandatory argument).\n";
        cout << endl;
        return 1;
    }

    /* Parse the configuration file. */
    XMLParser parser = XMLParser(arg);
    world = World(parser.get_node("world")); /* Set up the world. */

    glutInit(&argc, argv);
    world.initScene();

    glutInitWindowPosition(100, 100);
    glutCreateWindow("Model Viewer 3000");

    /* Put callback registry here. */
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(handleKey);
    glutSpecialFunc(handleSpecialKey);

    /* Enter GLUT's main loop. */
    glutMainLoop();

    return 1;
}
