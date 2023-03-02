#include "utils.hpp"
#include <cmath>

//Este ficheiro é a vergonha do Vasques

Vector zero() {
  return {0,0,0};
}

Vector one() {
  return {1,1,1};
}

Vector inverse(Vector v) {
  return {-std::get<0>(v),
          -std::get<1>(v),
          -std::get<2>(v)};
}

Vector scale(float n, Vector v) {
  return {n * std::get<0>(v),
          n * std::get<1>(v),
          n * std::get<2>(v)};
}

float length(Vector v) {
  return sqrt(dotProduct(v, v));
}

float square_length(Vector v) {
  return dotProduct(v, v);
}

Vector normalize(Vector v) {
  return scale(1 / length(v), v);
}

float angle(Vector u,Vector v){
  return acos(dotProduct(u,v)/(length(u)*length(v)));
}

//returns the vector from p1 to p2 (p2 - p1)
Vector difference(Point p1, Point p2) {
  return {std::get<0>(p2) - std::get<0>(p1),
          std::get<1>(p2) - std::get<1>(p1),
          std::get<2>(p2) - std::get<2>(p1)};
}

Point addVector(Point p, Vector v) {
  return {std::get<0>(p) + std::get<0>(v),
          std::get<1>(p) + std::get<1>(v),
          std::get<2>(p) + std::get<2>(v)};
}

float dotProduct(Vector u, Vector v) {
  return std::get<0>(u) * std::get<0>(v)
       + std::get<1>(u) * std::get<1>(v)
       + std::get<2>(u) * std::get<2>(v);
}

Vector crossProduct(Vector u, Vector v) {
  return {std::get<1>(u) * std::get<2>(v) - std::get<2>(u) * std::get<1>(v),
          std::get<2>(u) * std::get<0>(v) - std::get<0>(u) * std::get<2>(v),
          std::get<0>(u) * std::get<1>(v) - std::get<1>(u) * std::get<0>(v)};
}

//returns the projection of vector v onto vector u
Vector projectToVector(Vector u, Vector v) {
  return scale(dotProduct(u, v) / square_length(u), u);
}

//projects vector v to plane defined by normal vector n
Vector projectToPlane(Vector n, Vector v) {
  return addVector(v, inverse(projectToVector(n, v)));
}
Vector PerpendicularClockWiseByYAxis(Vector u){
  return {std::get<2>(u),std::get<1>(u),-std::get<0>(u)};
}
Vector PerpendicularAntiClockWiseByYAxis(Vector u){
  return {-std::get<2>(u),std::get<1>(u),std::get<0>(u)};
}

float randomFloat() {
  return (float)rand()/(float)(RAND_MAX);
}