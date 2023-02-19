#include <iostream>
#include <cstring>
#include <stdexcept>
#include "shape.hpp"
#include "shapegenerator.hpp"

unsigned constexpr shapetoint(char* shape) {
	//Hash copied from https://stackoverflow.com/a/2112111
	return *shape ?
        *shape + 33 * shapetoint(shape + 1) :
        5381;
}

inline std::unique_ptr<Shape> generateShape(char* argv[]) {
	switch(shapetoint(argv[1])) {
		case shapetoint((char*)"sphere"):
			return generateSphere(std::stof(argv[2]), std::stoi(argv[3]), std::stoi(argv[4]));
		case shapetoint((char*)"box"):
			return generateCube(std::stof(argv[2]), std::stoi(argv[3]));
		case shapetoint((char*)"cone"):
			return generateCone(std::stof(argv[2]), std::stof(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]));
		case shapetoint((char*)"plane"):
			return generatePlane(std::stof(argv[2]), std::stoi(argv[3]));
		default:
			throw std::invalid_argument("No such shape");
	}
}


int main(int argc, char* argv[]) {
	std::unique_ptr<Shape> shape = generateShape(argv);
	shape->exportToFile(argv[argc - 1]);
}