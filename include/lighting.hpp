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
  virtual void draw(int index) = 0;
};


class Lighting {
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

public:
  static bool accepts(XMLParser parser);

  PointLight(XMLParser parser);
  Light* clone();
  void draw(int index);
};

class DirectionalLight : public Light {
  Vector direction;

public:
  static bool accepts(XMLParser parser);

  DirectionalLight(XMLParser parser);
  Light* clone();
  void draw(int index);
};