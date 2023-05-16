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

float angle(Vector u,Vector v){
  /**
   * @brief Calculates the angle between two vectors
   * 
   * @param u the first vector
   * @param v the second vector
   * 
   * @return the angle between the two vectors in degrees
   */
  return acos(u * v / (length(u) * length(v)));
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
  return (u * v / square_length(u)) * u;
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
  return v - projectToVector(n, v);
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
  return (v * cossine)
        + (sinne * (axis ^ v))
        + ((1 - cossine) * (v * axis) * axis);
}

Vector getNormal(Triangle triangle) {
  Vector v1 = std::get<2>(triangle) - std::get<1>(triangle);
  Vector v2 = std::get<0>(triangle) - std::get<1>(triangle);
  Vector cross = v1 ^ v2;
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