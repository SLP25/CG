#pragma once

#include <tuple>
#include <initializer_list>

#define ONERAD 0.0174532925f

typedef std::tuple<int, int> WindowSize; ///< Tuple of a width and a height, both integers.

/**
 * @brief A tuple representing a point in 3D space
 */
typedef std::tuple<float, float, float> Point;

typedef std::tuple<Point, Point, Point> Triangle;

typedef Point Vector;


template<typename Iter> Point average(Iter begin, Iter end) {
  Point ans = {0,0,0};
  int n = 0;
  for (Iter it = begin; it != end; it++) {
    n++;
    ans = { std::get<0>(ans) + std::get<0>(*it),
            std::get<1>(ans) + std::get<1>(*it),
            std::get<2>(ans) + std::get<2>(*it)};
  }
  
  return { std::get<0>(ans) / n,
           std::get<1>(ans) / n,
           std::get<2>(ans) / n};
}

Point average(std::initializer_list<Point> points);
Vector zero();
Vector one();
Vector inverse(Vector v);
Vector scale(float n, Vector v);
float length(Vector v);
float square_length(Vector v);
float angle(Vector u,Vector v);
Vector normalize(Vector v);
Vector difference(Point p1, Point p2);
Point addVector(Point p, Vector v);
float dotProduct(Vector u, Vector v);
Vector crossProduct(Vector u, Vector v);
Vector projectToVector(Vector u, Vector v);
Vector projectToPlane(Vector n, Vector v);
Vector PerpendicularClockWiseByYAxis(Vector u);
Vector PerpendicularAntiClockWiseByYAxis(Vector u);
Vector RotateAroundXAxis(Vector u,float angle);
Vector RotateAroundYAxis(Vector u,float angle);
Vector RotateAroundZAxis(Vector u,float angle);