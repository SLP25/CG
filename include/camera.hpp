#pragma once

#include "parser.hpp"
#include "utils.hpp"
#include <iostream>

class Camera;

template <class... Cameras> struct helper {
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

private:

  /**
   * @brief Creates an object from the specified subclass of Camera that matches the name
   * 
   * @tparam Cameras The subclasses to search from. Must derive Camera and define a public static
   * std::string className() and a public constructor receiving a XMLParser
   * @param type    The name of the subclass to create
   * @param parser  The parser to pass to the constructor
   * @return std::unique_ptr<Camera> The constructed Camera object
   */
  template <class... Cameras>
  static std::unique_ptr<Camera> parseCamera(std::string type,
                                             XMLParser parser) {
    return helper<Cameras...>::parseCamera(type, parser);
  }
};

template <class Derived, class... Cameras> struct helper<Derived, Cameras...> {
  static std::unique_ptr<Camera> parseCamera(std::string type,
                                             XMLParser parser) {

    if (Derived::className() == type) {
      return std::make_unique<Derived>(parser);
    }

    return helper<Cameras...>::parseCamera(type, parser);
  }
};

template <> struct helper<> {
  static std::unique_ptr<Camera> parseCamera(__attribute__((unused))
                                             std::string type,
                                             __attribute__((unused))
                                             XMLParser parser) {
    throw std::exception(); // string("No listed camera matched the received
                            // type '") + type + string("'")
  }
};

/**
 * @brief A camera that moves in a sphere around the origin. The arrow keys move it around and up and down, and
 * the W and S keys move the camera closer and further away
 * 
 */
class PolarCamera : public Camera {
  Point lookAt;
  float angleXZ;
  float angleZY;
  float radius;
  Vector up;
  float fov;
  float near;
  float far;

public:
  static std::string className() { return "polar"; }

  PolarCamera(XMLParser parser);

  void changeSize(WindowSize windowSize);
  void setupScene();
  void handleSpecialKey(int key, int x, int y);
  void handleKey(unsigned char key, int x, int y);

private:
  Vector getPositionVector();
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
  static std::string className() { return "fps"; }

  FPSCamera(XMLParser parser);

  void changeSize(WindowSize windowSize);
  void setupScene();
  void handleKey(unsigned char key, int x, int y);
  void handleSpecialKey(int key, int x, int y);

    private:
        FPSCamera(Point,Vector,Vector,float,float,float);
};
