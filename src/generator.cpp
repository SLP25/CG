/**
 * @file generator.cpp
 * @brief File implementing the main generator program
 */
#include <stdlib.h>
#include "shape.hpp"
#include "shapegenerator.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>

/**
 * @brief Asserts that the number of arguments the program has received
 * equals the given number
 *
 * Will throw an @c invalid_argument exception if the number of arguments
 * differs from the expected
 *
 * @param n The expected number of arguments
 */
#define ASSERT_ARG_LENGTH(n)                                                   \
  if (argc != n)                                                               \
  throw std::invalid_argument("Wrong number of arguments")

/**
 * @brief A hash function for strings
 *
 * Used in order to be able to @c switch over a @c string value
 *
 * @param shape the string to hash
 * @return      the hash of the string
 */
unsigned constexpr shapetoint(char *shape) {
  // Hash copied from https://stackoverflow.com/a/2112111
  return *shape ? *shape + 33 * shapetoint(shape + 1) : 5381;
}

/**
 * @brief Generates the requested shape
 *
 * @param argc the number of arguments received
 * @param argv the arguments received
 *
 * @return the requested shape
 *
 * @throws invalid_argument if the arguments received do not follow the
 * specification
 */
std::unique_ptr<Shape> generateShape(int argc, char *argv[]) {
  switch (shapetoint(argv[1])) {
  case shapetoint((char *)"sphere"):
    ASSERT_ARG_LENGTH(6);
    return generateSphere(std::stof(argv[2]), std::stoi(argv[3]),
                          std::stoi(argv[4]));
  case shapetoint((char *)"box"):
    ASSERT_ARG_LENGTH(5);
    return generateCube(std::stof(argv[2]), std::stoi(argv[3]));
  case shapetoint((char *)"cone"):
    ASSERT_ARG_LENGTH(7);
    return generateCone(std::stof(argv[2]), std::stof(argv[3]),
                        std::stoi(argv[4]), std::stoi(argv[5]));
  case shapetoint((char *)"plane"):
    ASSERT_ARG_LENGTH(5);
    return generatePlane(std::stof(argv[2]), std::stoi(argv[3]));
  case shapetoint((char *)"cylinder"):
    ASSERT_ARG_LENGTH(6);
    return generateCylinder(std::stof(argv[2]), std::stof(argv[3]),
                            std::stof(argv[4]));
  case shapetoint((char *)"donut"):
    ASSERT_ARG_LENGTH(8);
    return generateDonut(std::stof(argv[2]), std::stof(argv[3]),
                         std::stof(argv[4]), std::stoi(argv[5]),
                         std::stoi(argv[6]));
  case shapetoint((char *)"obj"):
    ASSERT_ARG_LENGTH(4);
    return generateFromObj(argv[2]);
  default:
    throw std::invalid_argument("No such shape");
  }
}

/**
 * @brief Generator program entry point
 *
 * @param argc the number of arguments received
 * @param argv the arguments received
 *
 * @return 0 if execution was successful
 * @return 1 if an error occurred
 */
#ifndef ENGINE
int main(int argc, char *argv[]) {
  try {
    std::unique_ptr<Shape> shape = generateShape(argc, argv);
    if (!shape->exportToFile(argv[argc - 1])) {
      std::cout << "Error saving shape to file" << std::endl;
      return 1;
    };
    return 0;
  } catch (std::invalid_argument const &e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
}
#endif
