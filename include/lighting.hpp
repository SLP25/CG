#pragma once

/**
 * @file lighting.hpp
 * @brief File declaring the class responsible for lighting the world

*/

#include <vector>
#include "utils.hpp"
#include "parser.hpp"


class Light {
public:
  static std::unique_ptr<Light> parse(XMLParser parser);

  /**
   * @brief Default destructor
  */
  virtual ~Light();

  virtual Light* clone() = 0;
  virtual void draw(GLenum light) = 0;
};


class Lighting {
  Color ambient;
  std::vector<std::unique_ptr<Light>> lights;

public:
  Lighting();
  Lighting(XMLParser parser);
  Lighting(const Lighting&);
  Lighting& operator=(const Lighting&);
  void initScene();
  void setupScene();
};


class PointLight : public Light {
  Point pos;
  Color difuse;
  Color specular;

public:
  static bool accepts(XMLParser parser);

  PointLight(XMLParser parser);
  Light* clone();
  void draw(GLenum light);
};

class DirectionalLight : public Light {
  Vector direction;
  Color difuse;
  Color specular;

public:
  static bool accepts(XMLParser parser);

  DirectionalLight(XMLParser parser);
  Light* clone();
  void draw(GLenum light);
};

class SpotLight : public Light {
  Point position;
  Vector direction;
  float cutoff;
  Color difuse;
  Color specular;

public:
  static bool accepts(XMLParser parser);

  SpotLight(XMLParser parser);
  Light* clone();
  void draw(GLenum light);
};