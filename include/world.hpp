/**
 * @file world.hpp
 * @brief Defines the World class and its methods.
 */

#pragma once

#include <map>
#include <string>
#include <tuple>
#include <vector>
#include "model.hpp"
#include "parser.hpp"
#include "camera.hpp"

/**
 * @class World
 * @brief Represents a 3D world using OpenGL and GLUT.
 */
class World {

    WindowSize windowSize; ///< The size of the window.
    std::unique_ptr<Camera> camera; ///< The camera to use for rendering the scene.
    std::vector<Model> models; ///< The models to display in the scene.

public:
    /**
     * @brief Constructs a World object with default values.
     */
    World();

    /**
     * @brief Constructs a new World object from a xml node
     * 
     * @param parser The xml node
     */
    World(XMLParser parser);

    /**
     * @brief Initializes the OpenGL and GLUT environment.
     * @param argc A pointer to the number of command-line arguments.
     * @param argv An array of command-line argument strings.
     */
    void initScene();

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
     * @brief Constructs a World object with the given window size, camera, and models.
     * @param windowSize The size of the window to create.
     * @param camera The camera to use for rendering the scene.
     * @param models The models to display in the scene.
     */
    World(WindowSize windowSize, Camera* camera, std::vector<Model> models);

    /**
     * @brief Draws the x, y, and z axes.
     */
    void drawAxis();
};