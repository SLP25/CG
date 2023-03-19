#pragma once

#include <memory>

#include "parser.hpp"

class Transformation {
public:
  static std::unique_ptr<Transformation> parse(XMLParser parser);
  virtual ~Transformation();
  virtual Transformation* clone() = 0;
  virtual void apply() = 0;
};

class Translation : public Transformation {
public:
  Translation(XMLParser parser);
  Transformation* clone();
  void apply();

private:
  float x, y, z;
};

class Rotation : public Transformation {
public:
  Rotation(XMLParser parser);
  Transformation* clone();
  void apply();

private:
  float x, y, z, angle;
};

class Scale : public Transformation {
public:
  Scale(XMLParser parser);
  Transformation* clone();
  void apply();

private:
  float x, y, z;
};
