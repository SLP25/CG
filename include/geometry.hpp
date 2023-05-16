#pragma once

/**
 * @file geometry.hpp
 * @brief File declaring useful functions and types relating to math and geometry
*/

#include <tuple>

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
   * @brief Return the zero vector
   * @return {0,0,0}
   */
Vector zero();

/**
 * @brief Multiplies a vector by a scalar
 * 
 * @param x A scalar
 * @param v A vector
 * @return Vector The resulting vector
 */
Vector operator *(float x, const Vector& v);


/**
 * @brief Multiplies a vector by a scalar
 * 
 * @param v A vector
 * @param x A scalar
 * @return Vector The resulting vector
 */
Vector operator *(const Vector& v, float x);

/**
 * @brief Divides a vector by a scalar
 * 
 * @param v A vector
 * @param x A scalar
 * @return Vector The resulting vector
 */
Vector operator /(const Vector& v, float x);

/**
 * @brief Sums two vectors
 * 
 * @param v1 the first vector
 * @param v2 the second vector
 * 
 * @return the resulting vector
 */
Vector operator +(const Vector& v1, const Vector& v2);

/**
 * @brief Returns the inverse of the given vector, such that their sum
 * is the null vector
 * 
 * @param v The given vector
 * @return Vector The inverse
 */
Vector operator -(const Vector& v);

/**
 * @brief Returns the difference between two vectors
 * 
 * @param p1 A point
 * @param p2 Another one
 * @return Vector Bites the dust
 */
Vector operator -(const Vector& v1, const Vector& v2);

/**
 * @brief Calculates the dot product between two vectors
 * 
 * @param u A vector
 * @param v Another one
 * @return float Bytes the dust
 */
float operator *(const Vector& v1, const Vector& v2);

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
Vector operator ^(const Vector& v1, const Vector& v2);

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
 * @brief Returns a normalized vector (with length 1) with the same direction
 * 
 * @param v A vector
 * @return Vector A normalized vector
 */
Vector normalize(Vector v);