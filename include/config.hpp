/**
 * @file shape.hpp
 *
 * @brief File defining the @link Shape class
 */
#pragma once
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include "shape.hpp"

typedef std::tuple<int, int> WindowSize;
typedef std::tuple<float, float, float> Point;
typedef Point Position;
typedef Point LookAt;
typedef Point Up;
typedef Point Projection;
typedef std::tuple<Position, LookAt, Up, Projection> Camera;
typedef std::vector<Shape> Models;
typedef std::tuple<WindowSize,Camera,Models> Config;

