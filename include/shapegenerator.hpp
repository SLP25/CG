#pragma once
#include <memory>
#include "shape.hpp"

std::unique_ptr<Shape> generateSphere(float radius, int slices, int stacks);
std::unique_ptr<Shape> generateCone(float radius, float height, int slices, int stacks);
std::unique_ptr<Shape> generateCube(float length, int divisions);
std::unique_ptr<Shape> generatePlane(float length, int divisions);