/**
 * @file shapegenerator.hpp
 *
 * @brief File defining functions used to generate the different shapes
 */

#pragma once
#include "shape.hpp"
#include <memory>

/**
 * @brief Generates a sphere centered in the origin
 *
 * @param radius the radius of the sphere
 * @param slices the number of slices of the sphere
 * @param stacks the number of stacks of the sphere
 *
 * @return       the requested sphere
 */
std::unique_ptr<Shape> generateSphere(float radius, int slices, int stacks);

/**
 * @brief Generates a cone with its base on the 0xz axis
 *
 * @param radius the radius of the base of the cone
 * @param height the height of the cone
 * @param slices the number of slices of the cone
 * @param stacks the number of stacks of the cone
 *
 * @return       the requested cone
 */
std::unique_ptr<Shape> generateCone(float radius, float height, int slices,
                                    int stacks);

/**
 * @brief Generates a cube centered in the origin
 *
 * @param length    the length of the side of the cube
 * @param divisions the number of divisions of the cube
 *
 * @return          the requested cube
 */
std::unique_ptr<Shape> generateCube(float length, int divisions);
/**
 * @brief Generates a plane centered in the origin in the 0xz axis
 *
 * @param length    the length of the side of the plane
 * @param divisions the number of divisions of the plane
 *
 * @return          the requested plane
 */
std::unique_ptr<Shape> generatePlane(float length, int divisions);
