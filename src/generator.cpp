#include <iostream>
#include <cstring>
#include <stdexcept>
#include "shape.hpp"
#include "shapegenerator.hpp"

#define ASSERT_ARG_LENGTH(n) if(argc != n) throw std::invalid_argument("Wrong number of arguments")

unsigned constexpr shapetoint(char* shape) {
	//Hash copied from https://stackoverflow.com/a/2112111
	return *shape ?
        *shape + 33 * shapetoint(shape + 1) :
        5381;
}

inline std::unique_ptr<Shape> generateShape(int argc, char* argv[]) {
	switch(shapetoint(argv[1])) {
		case shapetoint((char*)"sphere"):
			ASSERT_ARG_LENGTH(6);
			return generateSphere(std::stof(argv[2]), std::stoi(argv[3]), std::stoi(argv[4]));
		case shapetoint((char*)"box"):
			ASSERT_ARG_LENGTH(5);
			return generateCube(std::stof(argv[2]), std::stoi(argv[3]));
		case shapetoint((char*)"cone"):
			ASSERT_ARG_LENGTH(7);
			return generateCone(std::stof(argv[2]), std::stof(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]));
		case shapetoint((char*)"plane"):
			ASSERT_ARG_LENGTH(5);
			return generatePlane(std::stof(argv[2]), std::stoi(argv[3]));
		default:
			throw std::invalid_argument("No such shape");
	}
}


int main(int argc, char* argv[]) {
	try {
		std::unique_ptr<Shape> shape = generateShape(argc, argv);
		if(!shape->exportToFile(argv[argc - 1])) {
			std::cout << "Error saving shape to file" << std::endl;
			return 1;
		};
		return 0;
	} catch(std::invalid_argument const& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
}