#pragma once

#include <tuple>
#include <initializer_list>

typedef std::tuple<int, int> WindowSize; ///< Tuple of a width and a height, both integers.

/**
 * @brief A tuple representing a point in 3D space
 */
typedef std::tuple<float, float, float> Point;

/**
 * @brief A tuple of 3 points in 3d space, representing a triangle
 * 
 */
typedef std::tuple<Point, Point, Point> Triangle;

/**
 * @brief Same as point, but is a vector
 * 
 */
typedef Point Vector;

/**
 * @brief Calculates the average of the points by adding their
 * components and dividing by the number of points
 * 
 * @tparam Iter The type of iterator
 * @param begin The begin iterator
 * @param end   The end iterator
 * @return Point The average point
 */
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

/**
 * @brief Calculates the average of the points by adding their
 * components and dividing by the number of points
 * 
 * @param points The list of points to average
 * @return Point The average point
 */
Point average(std::initializer_list<Point> points);

/**
 * @brief Creates a null vector
 * 
 * @return Vector A null vector
 */
Vector zero();

/**
 * @brief Creates a vector with one in each component
 * 
 * @return Vector A vector with one in each component
 */
Vector one();

/**
 * @brief Returns the inverse of the given vector, such that their sum
 * is the null vector
 * 
 * @param v The given vector
 * @return Vector The inverse
 */
Vector inverse(Vector v);

/**
 * @brief Multiplies a vector by a scalar
 * 
 * @param n A scalar
 * @param v A vector
 * @return Vector The resulting vector
 */
Vector scale(float n, Vector v);

/**
 * @brief Calculates the length of the vector
 * 
 * @param v The vector
 * @return float The length
 */
float length(Vector v);

/**
 * @brief Calculates the square of the length of the vector.
 * Is less expensive than length(), since to calculate the length
 * one must calculate the square and then apply the square root.
 * 
 * @param v 
 * @return float 
 */
float square_length(Vector v);

/**
 * @brief Calculates the angle between two vectors, in degrees
 * 
 * @param u A vector
 * @param v Another one
 * @return float Bites the dust
 */
float angle(Vector u,Vector v);

/**
 * @brief Returns a normalized vector (with length 1) with the same direction
 * 
 * @param v A vector
 * @return Vector A normalized vector
 */
Vector normalize(Vector v);

/**
 * @brief Returns a vector that defines a translation from p1 to p2
 * 
 * @param p1 A point
 * @param p2 Another one
 * @return Vector Bites the dust
 */
Vector difference(Point p1, Point p2);

/**
 * @brief Applies a translation to a point
 * 
 * @param p A point
 * @param v A vector defining a translation
 * @return Point The translated point
 */
Point addVector(Point p, Vector v);

/**
 * @brief Calculates the dot product between two vectors
 * 
 * @param u A vector
 * @param v Another one
 * @return float Bytes the dust
 */
float dotProduct(Vector u, Vector v);

/**
 * @brief Calculates the cross product between two vectors
 * Returns a vector perpendicular to the two given vectors, with the
 * direction according to the right hand rule and a length that equals the
 * product of the lengths of the two vectors when they are perpendicular and zero when
 * they are colinear (and somewhere in between for other angles).
 * 
 * @param u A vector
 * @param v Another one
 * @return float Bytes the dust
 */
Vector crossProduct(Vector u, Vector v);

/**
 * @brief Projects vector v onto vector u, returning a vector with the same
 * direction as u and a length that is maximum when the two vectors are colinear and
 * zero when they are perpendicular.
 * 
 * @param u The vector to projecto onto
 * @param v The vector to project
 * @return Vector The projected vector
 */
Vector projectToVector(Vector u, Vector v);

/**
 * @brief Projects a vector onto a plane
 * 
 * @param n The normal vector of the plane
 * @param v The vector
 * @return Vector The projected vector
 */
Vector projectToPlane(Vector n, Vector v);

Vector PerpendicularClockWiseByYAxis(Vector u);
Vector PerpendicularAntiClockWiseByYAxis(Vector u);
Vector rotate(Vector around,Vector v,float angle);