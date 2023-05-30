#pragma once

#include "parser.hpp"
#include "utils.hpp"
#include <iostream>


/**
 * @brief Represents the camera through which the world is viewed. It stores information about the camera's
 * position, direction, frustum and how these elements respond to inputs.
 * 
 */
class Camera {

protected:
  Point lookAt;
  Point position;
  Vector up;

  float near;
  float far;
  float fov;
  float ratio;

public:
  /**
   * @brief Creates a Camera object by reading from a xml node. This works by searching the type attribute of
   * the xml node to find which Camera subclass to use. The xml node is then passed to the subclass constructor
   * 
   * @param parser The xml node
   * @return std::unique_ptr<Camera> The constructed Camera object
   */
  static std::unique_ptr<Camera> parse(XMLParser parser);

  /**
  * @brief A virtual destructor for the Camera class.
  */
  ~Camera();

  /**
   * @brief Is called whenever the window changes size. Is used to adjust the fov
   * 
   * @param windowSize The new dimensions of the window
   */
  void changeSize(WindowSize windowSize);

  /**
   * @brief Is called before rendering the scene
   * 
   */
  void setupScene();

  /**
   * @brief Constructs the view frustum
   * 
   * @return The view frustum
   * 
   */
  Frustum viewFrustum();

  /**
   * @brief Is called whenever a character key is pressed
   * 
   * @param key The pressed key
   * @param x   The x position of the mouse
   * @param y   The y position of the mouse
   */
  virtual void handleKey(unsigned char key, int x, int y);

  /**
   * @brief Is called whenever a special key is pressed
   * 
   * @param key The pressed key's code
   * @param x   The x position of the mouse
   * @param y   The y position of the mouse
   */
  virtual void handleSpecialKey(int key, int x, int y);
};


/**
 * @brief A camera that moves in a sphere around the origin. The arrow keys move it around and up and down, and
 * the W and S keys move the camera closer and further away
 * 
 */
class PolarCamera : public Camera {

public:

  /**
   * @brief Determines whether this class accepts to construct an instance
   * with the given parser
   * @return The parser
   */
  static bool accepts(XMLParser parser);

  /**
   * @brief Constructs a new PolarCamera object from the provided XML parser
   * @param parser The XML parser used for constructing the camera
   */
  PolarCamera(XMLParser parser);

  /**
   * @brief Handles keyboard keys for camera movement
   * @param key The key code
   * @param x The x-coordinate of the mouse
   * @param y The y-coordinate of the mouse
   */
  void handleKey(unsigned char key, int x, int y);

  /**
   * @brief Handles special keyboard keys for camera movement
   * @param key The special key code
   * @param x The x-coordinate of the mouse
   * @param y The y-coordinate of the mouse
   */
  void handleSpecialKey(int key, int x, int y);
};

/**
 * @brief Moves the camera as in an fps game. The arrow keys work as a mouse, rotating the camera left-right
 * and up-down, WASD move it perpendicularly to the up direction and ZX move it up and down
 * 
 */
class FPSCamera : public Camera {

public:

    /**
     * @brief Determines whether this class accepts to construct an instance
     * with the given parser
     * @return The parser
     */
    static bool accepts(XMLParser parser);

    /**
     * @brief Constructs a new FPSCamera object from the provided XML parser
     * @param parser The XML parser used for constructing the camera
     */
    FPSCamera(XMLParser parser);

    /**
     * @brief Handles keyboard keys for camera movement
     * @param key The key code
     * @param x The x-coordinate of the mouse
     * @param y The y-coordinate of the mouse
     */
    void handleKey(unsigned char key, int x, int y);

    /**
     * @brief Handles special keyboard keys for camera movement
     * @param key The special key code
     * @param x The x-coordinate of the mouse
     * @param y The y-coordinate of the mouse
     */
    void handleSpecialKey(int key, int x, int y);
};