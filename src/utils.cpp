#include "utils.hpp"

//Este ficheiro é a vergonha do Vasques

Point addVector(Point p, Vector v) {
  return {std::get<0>(p) + std::get<0>(v),
          std::get<1>(p) + std::get<1>(v),
          std::get<2>(p) + std::get<2>(v)};
}

float randomFloat() {
  return (float)rand()/(float)(RAND_MAX);
}