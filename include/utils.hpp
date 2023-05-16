#pragma once

/**
 * @file utils.hpp
 * @brief File declaring useful functions and types for the rest of the program
*/

#include <tuple>
#include <initializer_list>
#include <memory>
#include <string>

#include "geometry.hpp"
#include "parser.hpp"
#include "exceptions/invalid_xml_file.hpp"

#define GET_ALL(tuple) std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple) 

typedef std::tuple<int, int> WindowSize; ///< Tuple of a width and a height, both integers.


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
    ans = ans + *it;
  }
  
  return ans / n;
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
 * @brief Calculates the angle between two vectors, in degrees
 * 
 * @param u A vector
 * @param v Another one
 * @return float Bites the dust
 */
float angle(Vector u,Vector v);

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
 * @brief calculate the normal vector of the triangle
 * 
 * @param triangle the given triangle
 * 
 * @return the normal vector
*/
Vector getNormal(Triangle triangle);

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


/**
 * @brief A generic matrix multiplication function.
 * 
 * It requires the product (* operator) to be defined for the types
 * being multiplied, as well as addition (+ operator) for the type
 * of the output matrix.
 * 
 * It assumes the null (zero) value of the type
 * is one where the memory is all set to zero.
 * 
 * @param n The number of columns of the output matrix
 * @param m The number of rows of the output matrix
 * @param l The number of columns of the first input matrix (the
 * same as the number of rows of the second input matrix)
 * @param a The first input matrix
 * @param b The second input matrix
 * @param c The input matrix
*/
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
