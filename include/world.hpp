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
#include "group.hpp"
#include "lighting.hpp"

/**
 * @class World
 * @brief Represents a 3D world using OpenGL and GLUT.
 */
class World {
private:
    std::string srcFile; ///< The xml file from which the World was read
    WindowSize windowSize; ///< The size of the window.
    std::unique_ptr<Camera> camera; ///< The camera to use for rendering the scene.
    Lighting lighting; ///< The lighting to use for rendering the scene.
    Group root; ///< The root group of the scene.
    bool axis; ///< Whether to draw the axis

    /**
     * @brief Constructs a World object with the given window size, camera, and group.
     * @param windowSize The size of the window to create.
     * @param camera The camera to use for rendering the scene.
     * @param lighting The lighting of the scene
     * @param models The root group of the scene.
     */
    World(WindowSize windowSize, Camera* camera, Lighting lighting, Group&& root);

    /**
     * @brief Draws the x, y, and z axes.
     */
    void drawAxis();

    XMLParser parseXMLFile(std::string filePath);
    void parseWindow(XMLParser world);
    void parseCamera(XMLParser world);
    void parseLights(XMLParser world);
    void parseRootGroup(XMLParser world);

public:
    /**
     * @brief Constructs a World object with default values.
     */
    World();

    /**
     * @brief Constructs a new World object from a xml file
     * 
     * @param parser The xml file path
     */
    World(std::string filePath);

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
};