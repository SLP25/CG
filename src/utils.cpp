#include "utils.hpp"
#include <cmath>
#include <string>


Point average(std::initializer_list<Point> points) {
  /**
  * @brief Calculates the average of the points by adding their
  * components and dividing by the number of points
  * 
  * @param points the list of points to calculate the average
  * @return Point The average point
  */
  return average(points.begin(), points.end());
}


Point operator *(float x, Point p) {
  return scale(x, p);
}

Point operator *(Point p, float x) {
  return scale(x, p);
}

Point operator +(Point p1, Point p2) {
  return addVector(p1, p2);
}

Vector zero() {
  /**
   * @brief Return the zero vector
   * @return {0,0,0}
   */
  return {0,0,0};
}

Vector one() {
  /**
   * @brief Returns the Unit vector
   * @return {1,1,1}
   */
  return {1,1,1};
}

Vector inverse(Vector v) {
  /**
   * @brief calculates the inverse of a vector
   * 
   * @param v the vector to calculate the inverse
   * 
   * @return the inverse of a vector
   */
  return {-std::get<0>(v),
          -std::get<1>(v),
          -std::get<2>(v)};
}

Vector scale(float n, Vector v) {
  /**
   * @brief calculates the vector scaled by a given factor
   * 
   * @param n the factor to scale by
   * @param v the vector to scale
   * 
   * @return the scaled vector
   */
  return {n * std::get<0>(v),
          n * std::get<1>(v),
          n * std::get<2>(v)};
}

float length(Vector v) {
  /**
   * @brief Calculates the legth of a vector
   * 
   * @param v the vector to calculate the length
   * 
   * @return the length of the vector
   */
  return sqrt(dotProduct(v, v));
}

float square_length(Vector v) {
  /**
   * @brief calculates the length squared value of a vector
   * 
   * @param v the vector to calculate the length squared
   * 
   * @return the length squared of the vector
   */
  return dotProduct(v, v);
}

Vector normalize(Vector v) {
  /**
   * @brief Calculates the normalized version of a vector
   * 
   * @param v the vector to calculate the normalized versions
   * 
   * @return the vector with length 1 and the same direction as the given vector
   */
  return scale(1 / length(v), v);
}

float angle(Vector u,Vector v){
  /**
   * @brief Calculates the angle between two vectors
   * 
   * @param u the first vector
   * @param v the second vector
   * 
   * @return the angle between the two vectors in degrees
   */
  return acos(dotProduct(u,v)/(length(u)*length(v)));
}

Vector difference(Point p1, Point p2) {
  /**
   * @brief Calculates the vector from p1 to p2
   * 
   * @param p1 the starting point of the vector
   * @param p2 the ending point of the vector
   * 
   * @return the vector from p1 to p2
   * 
   */
  return {std::get<0>(p2) - std::get<0>(p1),
          std::get<1>(p2) - std::get<1>(p1),
          std::get<2>(p2) - std::get<2>(p1)};
}

Point addVector(Point p, Vector v) {
  /**
   * @brief Applies a vector to a point (can also be used to sum to vectors)
   * 
   * @param p the point to apply the vector to
   * @param v the vector to apply
   * 
   * @return the point after applying the vector to the point
   */
  return {std::get<0>(p) + std::get<0>(v),
          std::get<1>(p) + std::get<1>(v),
          std::get<2>(p) + std::get<2>(v)};
}

float dotProduct(Vector u, Vector v) {
  /**
   * @brief calculates the dot product between two vectors
   * 
   * @param u the first vector
   * @param v the second vector
   * 
   * @return the dot product between the vectors
   * 
   */
  return std::get<0>(u) * std::get<0>(v)
       + std::get<1>(u) * std::get<1>(v)
       + std::get<2>(u) * std::get<2>(v);
}

Vector crossProduct(Vector u, Vector v) {
  /**
   * @brief calculates the cross product between two vectors
   * 
   * @param u the first vector
   * @param v the second vector
   * 
   * @return the cross product between the vectors
   * 
   */
  return {std::get<1>(u) * std::get<2>(v) - std::get<2>(u) * std::get<1>(v),
          std::get<2>(u) * std::get<0>(v) - std::get<0>(u) * std::get<2>(v),
          std::get<0>(u) * std::get<1>(v) - std::get<1>(u) * std::get<0>(v)};
}

Vector projectToVector(Vector u, Vector v) {
  /**
   * @brief calculates the projection of a vector onto another vector
   * 
   * @param u the vector to project to
   * @param v the vector to be projected
   * 
   * @return the projection of vector v onto vector u
   * 
   */
  return scale(dotProduct(u, v) / square_length(u), u);
}

Vector projectToPlane(Vector n, Vector v) {
  /**
   * @brief calculates the projection of a vector onto a plane
   * 
   * @param u the normal vector of the plane to project to
   * @param v the vector to be projected
   * 
   * @return the projection of vector v onto the plane with normal vector u
   * 
   */
  return addVector(v, inverse(projectToVector(n, v)));
}

float randomFloat() {
  /**
   * @brief returns a random float
   * 
   * @return a random float
   */
  return (float)rand()/(float)(RAND_MAX);
}


Vector rotate(Vector around,Vector v,float angle){
  /**
   * @brief calculate the resulting vector of rotating a vector around another vector by a given angle
   * 
   * @param around the vector to rotate around
   * @param v the vector to rotate
   * @param angle the angle to rotate by in degrees
   * 
   * @return the vector resulting of rotating v angle degrees around around
   */
  Vector axis = normalize(around);
  float cossine = cos(angle);
  float sinne = sin(angle);
  float dot = dotProduct(v,axis);
  Vector cross = crossProduct(axis, v);
  return addVector(addVector(
          scale(cossine,v),
          scale(sinne,cross)),
          scale((1 - cossine) * dot,axis));
}

Vector getNormal(Triangle triangle) {
  Vector v1 = difference(std::get<1>(triangle), std::get<0>(triangle));
  Vector v2 = difference(std::get<1>(triangle), std::get<2>(triangle));
  Vector cross = crossProduct(v2, v1);
  return cross == zero() ? zero() : normalize(cross);
}

std::tuple<float, float, float> parseHexColor(std::string colorHex) {
  /**
   * @brief Converts a string containing a hex representation of a color into rgb
   * @return A tuple with values of {red,green,blue}
   */
  unsigned int rgb[3];
  for(int i = 0; i < 3; i++) {
    std::stringstream ss;
    ss << std::hex << colorHex.substr(1 + i * 2,2);
    ss >> rgb[i];
  }

  return {(float)rgb[0] / 255.0f, (float)rgb[1] / 255.0f, (float)rgb[2] / 255.0f};
}

bool parseBool(std::string str) {
  for (char& c : str)
    c = std::tolower(c);

  if (str == "true")
    return true;

  if (str == "false")
    return false;

  throw InvalidXMLStructure("Input '" + str + "' isn't a boolean");
}