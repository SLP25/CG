#include "shape.hpp"
#include <fstream>

Shape::Shape() {
    points = std::vector<Point>();
}

void Shape::exportToFile(std::string filePath) {
    std::ofstream file(filePath);
    file << "Hello World";
    file.close();
}