#pragma once
#include <string>
#include <tuple>
#include <vector>

// TODO: Refactor to class if needed
typedef std::tuple<float, float, float> Point;

class Shape {
public:
  Shape();
  bool exportToFile(std::string filePath);

private:
  std::vector<Point> points;
};
