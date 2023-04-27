#include "geometry.hpp"
#include <cmath>

Vector zero() {
  return {0,0,0};
}


Vector operator *(float x, const Vector& v) {
  return {x * std::get<0>(v),
          x * std::get<1>(v),
          x * std::get<2>(v)};
}

Vector operator *(const Vector& v, float x) {
  return x * v;
}

Vector operator /(const Vector& v, float x) {
    return {std::get<0>(v) / x,
            std::get<1>(v) / x,
            std::get<2>(v) / x};
}

Vector operator +(const Vector& v1, const Vector& v2) {
  return {std::get<0>(v1) + std::get<0>(v2),
          std::get<1>(v1) + std::get<1>(v2),
          std::get<2>(v1) + std::get<2>(v2)};
}

Vector operator -(const Vector& v) {
   return {-std::get<0>(v),
           -std::get<1>(v),
           -std::get<2>(v)};
}

Vector operator -(const Vector& v1, const Vector& v2) {
    return {std::get<0>(v1) - std::get<0>(v2),
            std::get<1>(v1) - std::get<1>(v2),
            std::get<2>(v1) - std::get<2>(v2)};
}

float operator *(const Vector& v1, const Vector& v2) {
    return std::get<0>(v1) * std::get<0>(v2)
        + std::get<1>(v1) * std::get<1>(v2)
        + std::get<2>(v1) * std::get<2>(v2);
}

Vector operator ^(const Vector& v1, const Vector& v2) {
    return {std::get<1>(v1) * std::get<2>(v2) - std::get<2>(v1) * std::get<1>(v2),
            std::get<2>(v1) * std::get<0>(v2) - std::get<0>(v1) * std::get<2>(v2),
            std::get<0>(v1) * std::get<1>(v2) - std::get<1>(v1) * std::get<0>(v2)};
}

float length(Vector v) {
  /**
   * @brief Calculates the legth of a vector
   * 
   * @param v the vector to calculate the length
   * 
   * @return the length of the vector
   */
  return sqrt(v * v);
}

float square_length(Vector v) {
  /**
   * @brief calculates the length squared value of a vector
   * 
   * @param v the vector to calculate the length squared
   * 
   * @return the length squared of the vector
   */
  return v * v;
}

Vector normalize(Vector v) {
  /**
   * @brief Calculates the normalized version of a vector
   * 
   * @param v the vector to calculate the normalized versions
   * 
   * @return the vector with length 1 and the same direction as the given vector
   */
  return (1 / length(v)) * v;
}