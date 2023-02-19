#include "shapegenerator.hpp"
#include <iostream>

std::unique_ptr<Shape> generateSphere(float radius, int slices, int stacks) {
  std::cout << (radius) / (slices + stacks); // So para compilar, remover depois
  return std::make_unique<Shape>();
}

std::unique_ptr<Shape> generateCone(float radius, float height, int slices,
                                    int stacks) {
  std::cout << (radius + height) /
                   (slices + stacks); // So para compilar, remover depois
  return std::make_unique<Shape>();
}

std::unique_ptr<Shape> generateCube(float length, int divisions) {
  std::cout << length / divisions; // So para compilar, remover depois
  return std::make_unique<Shape>();
}

std::unique_ptr<Shape> generatePlane(float length, int divisions) {
  std::cout << length / divisions; // So para compilar, remover depois
  return std::make_unique<Shape>();
}
