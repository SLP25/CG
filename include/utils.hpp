#pragma once

/**
 * @file utils.hpp
 * @brief File declaring useful functions and types for the rest of the program
*/

#include <tuple>
#include <initializer_list>
#include <memory>
#include <string>

#include "parser.hpp"
#include "exceptions/invalid_xml_file.hpp"

typedef std::tuple<int, int> WindowSize; ///< Tuple of a width and a height, both integers.

/**
 * @brief A tuple representing a point in 3D space
 */
typedef std::tuple<float, float, float> Point;

Point operator *(float x, Point p);
Point operator *(Point p, float x);
//Point operator *(float x, std::tuple<float, float, float> p);
//Point operator *(std::tuple<float, float, float> p, float x);

Point operator *(float x, Point p);

Point operator *(Point p, float x);

Point operator +(Point p1, Point p2);

//Point operator +(std::tuple<float, float, float> p1, Point p2);

Point operator +(Point p1, std::tuple<float, float, float> p2); 

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

/**
* @brief calculate the resulting vector of rotating a vector around another vector by a given angle
*
* @param around the vector to rotate around
* @param v the vector to rotate
* @param angle the angle to rotate by in degrees
*
* @return the vector resulting of rotating v angle degrees around around
*/
Vector rotate(Vector around,Vector v,float angle);

/**
   * @brief Casts an hexadecimal string (format: "#[0-9a-f]{6}") to the
   * corresponding RGB values.
   * 
   * The values range from [0-1]. 1 corresponds to all of that color
   * (255) and 0 to no color (0)
   * 
   * @note No verification is being done to the format of the string. The
   * output of this method is undefined for malformatted strings
   * 
   * @param color the hexadecimal string (format: "#[0-9a-f]{6}")
   * 
   * @returns a tuple with 3 floats:
   * 0 -> the red part of the color
   * 1 -> the green part of the color
   * 2 -> the blue part of the color
  */
std::tuple<float, float, float> parseHexColor(std::string color);

/**
 * @brief Parses the given string into true if the input is "true" and
 * false if the input is "false". If the input is neither, an error is thrown.
 * 
 * @note The input is case insensitive
*/
bool parseBool(std::string str);

/**
 * @brief A template struct to parse different types of classes using an XML parser.
 * @tparam Superclass The superclass of the possible classes
 * @tparam Subclasses Variadic template parameter pack of possible classes to search.
*/
template <class Superclass, class... Subclasses> struct dynamicParser {

  /**
   * @brief Parses an object of a given class using the provided XML parser.
   * @param parser The XML parser to use for parsing
   * @return A unique pointer to the parsed object
  */
  static std::unique_ptr<Superclass> parse(XMLParser parser);
};

template <class Superclass, class Subclass, class... Subclasses> struct dynamicParser<Superclass, Subclass, Subclasses...> {

 /**
 * @brief Parses an object of a given class using the provided XML parser
 * @param parser The XML parser to use for parsing
 * @return A unique pointer to the parsed camera object
 */
  static std::unique_ptr<Superclass> parse(XMLParser parser) {

    if (Subclass::accepts(parser)) {
      return std::make_unique<Subclass>(parser);
    }

    return dynamicParser<Superclass, Subclasses...>::parse(parser);
  }
};

/**
 * @brief A specialization of the dynamicParser struct for when the search for when no match is found
 */
template <class Superclass> struct dynamicParser<Superclass> {

  /**
   * @brief Throws an exception since no matches were found
   * @param parser Unused attribute
   * @return Throws an exception
   */
  static std::unique_ptr<Superclass> parse(XMLParser parser) {
    throw InvalidXMLStructure(std::string("No listed class accepted received node <")
                              + parser.name() + std::string(">"));
  }
};


//n, m -> columns and rows of output matrix
//l -> columns of first/rows of second input matrix
//a, b -> input matrices
//c ->output matrix
template <typename T, typename S, typename U> 
void matrixProd(int n, int m, int l, T* a, S* b, U *c) {
  for (int y = 0; y < m; y++) {
    for (int x = 0; x < n; x++) {
      c[n * y + x] = U{};
      for (int i = 0; i < l; i++)
        c[n * y + x] = c[n * y + x] + (a[l * y + i] * b[n * i + x]);
    }
  }
};
