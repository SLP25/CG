/**
 * @file config.hpp
 * @brief Defines the World class and its methods.
 */

#pragma once

#include <map>
#include <string>
#include <tuple>
#include <vector>
#include "shape.hpp"

typedef std::tuple<int, int> WindowSize; ///< Tuple of a width and a height, both integers.
typedef std::tuple<float, float, float> Point; ///< Tuple of three coordinate components, all floats.

typedef Point Position, LookAt, Up, Projection;

typedef std::tuple<Position, LookAt, Up, Projection> Camera; ///< Camera object.
typedef std::vector<Shape> Models; ///< Vector of shapes that constitutes the models.

/**
 * @class World
 * @brief Represents a 3D world using OpenGL and GLUT.
 */
class World {

    WindowSize windowSize; ///< The size of the window.
    Camera camera; ///< The camera to use for rendering the scene.
    Models models; ///< The models to display in the scene.

public:

    /**
     * @brief Constructs a World object with the given window size, camera, and models.
     * @param windowSize The size of the window to create.
     * @param camera The camera to use for rendering the scene.
     * @param models The models to display in the scene.
     */
    World(WindowSize windowSize, Camera camera, Models models);

    /**
     * @brief Constructs a World object with default values.
     */
    World();

    /**
     * @brief Initializes the OpenGL and GLUT environment.
     * @param argc A pointer to the number of command-line arguments.
     * @param argv An array of command-line argument strings.
     */
    void initScene(int* argc, char** argv);

    /**
     * @brief Handles a change in the window size.
     * @param width The new width of the window.
     * @param height The new height of the window.
     */
    void changeSize(int width, int height);

    /**
     * @brief Renders the scene using OpenGL and GLUT.
     */
    void renderScene();

    /**
     * @brief Handles a keyboard input event.
     * @param key The ASCII code of the key that was pressed.
     * @param x The x-coordinate of the mouse at the time of the event.
     * @param y The y-coordinate of the mouse at the time of the event.
     */
    void handleKey(unsigned char key, int x, int y);

    /**
     * @brief Handles a special key input event.
     * @param key The GLUT constant representing the special key that was pressed.
     * @param x The x-coordinate of the mouse at the time of the event.
     * @param y The y-coordinate of the mouse at the time of the event.
     */
    void handleSpecialKey(int key, int x, int y);

private:

    /**
     * @brief Draws the x, y, and z axes.
     */
    void drawAxis();
};