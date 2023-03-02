#pragma once

#include <tuple>

#define ONERAD 0.0174532925f

typedef std::tuple<int, int> WindowSize; ///< Tuple of a width and a height, both integers.

/**
 * @brief A tuple representing a point in 3D space
 */
typedef std::tuple<float, float, float> Point;

typedef std::tuple<Point, Point, Point> Triangle;

typedef Point Vector;


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