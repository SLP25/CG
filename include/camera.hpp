#pragma once

#include "parser.hpp"
#include "utils.hpp"
#include <iostream>

class Camera;

/**
 * @brief A helper struct to parse different types of cameras using the XML parser.
 * @tparam Cameras Variadic template parameter pack of camera types.
*/
template <class... Cameras> struct helper {

  /**
   * @brief Parses a camera of a given type using the provided XML parser.
   * @param type The type of camera to parse
   * @param parser The XML parser to use for parsing
   * @return A unique pointer to the parsed camera object
  */
  static std::unique_ptr<Camera> parseCamera(std::string type,
                                             XMLParser parser);
};

/**
 * @brief Represents the camera through which the world is viewed. It stores information about the camera's
 * position, direction, frustrum and how these elements respond to inputs.
 * 
 */
class Camera {

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
  virtual ~Camera();

  /**
   * @brief Is called whenever the window changes size. Is used to adjust the fov
   * 
   * @param windowSize The new dimensions of the window
   */
  virtual void changeSize(WindowSize windowSize) = 0;

  /**
   * @brief Is called before rendering the scene
   * 
   */
  virtual void setupScene() = 0;

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

protected:
  /**
   * @brief Updates the window viewport and the frustrum of the camera based on the given valiues
   * 
   * @param windowSize  The size of the window
   * @param fov         The field of view (in degrees)
   * @param near        The begin of the frustrum
   * @param far         The end of the frustrum
   */
  static void defaultChangeSize(WindowSize windowSize, float fov, float near,
                                float far);
};


/**
 * @brief A camera that moves in a sphere around the origin. The arrow keys move it around and up and down, and
 * the W and S keys move the camera closer and further away
 * 
 */
class PolarCamera : public Camera {

  Point lookAt;
  Point position;

  Vector up;

  float fov;
  float near;
  float far;

public:

  /**
   * @brief Returns the class name of the PolarCamera
   * @return The class name "polar"
   */
  static std::string className() { return "polar"; }

  /**
   * @brief Constructs a new PolarCamera object from the provided XML parser
   * @param parser The XML parser used for constructing the camera
   */
  PolarCamera(XMLParser parser);

  /**
   * @brief Changes the size of the camera viewport
   * @param windowSize The new size of the window
   */
  void changeSize(WindowSize windowSize);

  /**
   * @brief Sets up the camera view and projection matrices for the current scene
   */
  void setupScene();

  /**
   * @brief Handles special keyboard keys for camera movement
   * @param key The special key code
   * @param x The x-coordinate of the mouse
   * @param y The y-coordinate of the mouse
   */
  void handleSpecialKey(int key, int x, int y);

  /**
   * @brief Handles keyboard keys for camera movement
   * @param key The key code
   * @param x The x-coordinate of the mouse
   * @param y The y-coordinate of the mouse
   */
  void handleKey(unsigned char key, int x, int y);
};

/**
 * @brief Moves the camera as in an fps game. The arrow keys work as a mouse, rotating the camera left-right
 * and up-down, WASD move it perpendicularly to the up direction and ZX move it up and down
 * 
 */
class FPSCamera : public Camera {

    Point position;
    Vector lookAtVector;
    Vector up;

    float fov;
    float near;
    float far;

public:

    /**
     * @brief Returns the class name of the FPSCamera
     * @return The class name "fps"
     */
    static std::string className() { return "fps"; }

    /**
     * @brief Constructs a new FPSCamera object from the provided XML parser
     * @param parser The XML parser used for constructing the camera
     */
    FPSCamera(XMLParser parser);

    /**
     * @brief Changes the size of the camera viewport
     * @param windowSize The new size of the window
     */
    void changeSize(WindowSize windowSize);

    /**
     * @brief Sets up the camera view and projection matrices for the current scene
     */
    void setupScene();

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

private:

    /**
     * @brief Constructs an FPSCamera object with the provided parameters
     * @param position The initial position of the camera
     * @param lookAtVector The initial look-at vector of the camera
     * @param up The up vector of the camera
     * @param fov The field of view of the camera
     * @param near The near plane distance of the camera
     * @param far The far plane distance of the camera
     */
    FPSCamera(Point, Vector, Vector, float, float, float);
};