#pragma once

/**
 * @file transformation.hpp
 * @brief File declaring the classes representing the various transformations that
 * can be applied to a scene (scale, rotation and translation)
*/

#include <memory>

#include "parser.hpp"

/**
 * @brief An abstract class to generically represent a transformation
*/
class Transformation {
public:

  /**
   * @brief Creates a #Transformation from the XML string.
   * 
   * This method checks the type of transformation, and delegates
   * the creation to the respective subclass. This allows for the callee
   * to not need to worry about the type of transformation it wants to create
   * 
   * @param parser the parser for the XML string
   * 
   * @returns a unique pointer to the #Transformation
   * 
   * @throws If the string is not valid, an #InvalidXMLStructure exception
   * is thrown
  */
  static std::unique_ptr<Transformation> parse(XMLParser parser);

  /**
   * @brief Default destructor
  */
  virtual ~Transformation();

  /**
   * @brief Default clone method
  */
  virtual Transformation* clone() = 0;

  /**
   * @brief Applies a transformation to the scene
  */
  virtual void apply() = 0;
};

/**
 * @brief A translation
*/
class Translation : public Transformation {
public:

  /**
   * @brief Determines whether this class accepts to construct an instance
   * with the given parser
   * @return The parser
   */
  static bool accepts(XMLParser parser);

  Translation(XMLParser parser);
  Transformation* clone();
  void apply();

private:
  /**
   * @brief The x component of the translation vector
  */
  float x;
  /**
   * @brief The y component of the translation vector
  */
  float y;
  /**
   * @brief The z component of the translation vector
  */
  float z;
};

/**
 * @brief A rotation
*/
class Rotation : public Transformation {
public:

  /**
   * @brief Determines whether this class accepts to construct an instance
   * with the given parser
   * @return The parser
   */
  static bool accepts(XMLParser parser);

  Rotation(XMLParser parser);
  Transformation* clone();
  void apply();

private:
  /**
   * @brief The x component of the vector to rotate around
  */
  float x;
  /**
   * @brief The y component of the vector to rotate around
  */
  float y;
  /**
   * @brief The z component of the vector to rotate around
  */
  float z;
  /**
   * @brief The time (in seconds) for the rotation to complete (360 degrees).
   * If time is zero, there is no animation (as if time = +INF)
  */
  float time;
  /**
   * @brief The angle (in degrees) of the rotation.
   * If time is non-zero, angle serves as the initial rotation
  */
  float angle;
};

/**
 * @brief A scale
*/
class Scale : public Transformation {
public:

  /**
   * @brief Determines whether this class accepts to construct an instance
   * with the given parser
   * @return The parser
   */
  static bool accepts(XMLParser parser);

  Scale(XMLParser parser);
  Transformation* clone();
  void apply();

private:
  /**
   * @brief the scale factor in the x direction
  */
  float x;
  /**
   * @brief the scale factor in the y direction
  */
  float y;
  /**
   * @brief the scale factor in the z direction
  */
  float z;
};


/**
 * @brief A Catmull-Rom curve
*/
class CatmullRom : public Transformation {
public:
  /**
   * @brief Determines whether this class accepts to construct an instance
   * with the given parser
   * @return The parser
   */
  static bool accepts(XMLParser parser);

  CatmullRom(XMLParser parser);
  Transformation* clone();
  void apply();
  void draw();

private:
  /**
   * @brief the period of the animation
  */
  float time;

  /**
   * @brief whether the frame should be aligned in the direction of the motion
  */
  bool align;

  /**
   * @brief if true, draws a trace of the curve when applied by cutting each segment into 10.
   * Performance intensive
  */
  bool trace;

  /**
   * @brief the list of points forming the curve
  */
  std::vector<Point> points;


  Vector y_0 = {0, 1, 0};


  void getGlobalCatmullRomPoint(float gt, Point& pos, Vector& deriv);
};