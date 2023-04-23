/**
 * @file shape.cpp
 *
 * @brief File implementing the @link Shape class
 */

#include "glut.hpp"
#include "shape.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "exceptions/invalid_xml_file.hpp"
#include <map>
#include <tuple>

std::map<std::string,std::shared_ptr<Shape>> Shape::cache;

std::shared_ptr<Shape> Shape::fetchShape(std::string filePath) {
  
  if (cache.find(filePath) != cache.end())
    return cache[filePath];

  std::shared_ptr<Shape> s = std::shared_ptr<Shape>(new Shape(filePath));
  cache[filePath] = s;
  return s;
}

void Shape::initShapes() {
  for (auto const& ss : cache)
    ss.second->initialize();
}


Shape::Shape() {}

Shape::Shape(std::vector<Triangle> triangles) : vbo_addr(0) {
  //points found in the vector of triangles and the position they will be stored in the points vector
  std::map<Point, int> pointsFound; 
  Point points[3];
  int pos[3];
  
  for (Triangle &triangle : triangles) {
    points[0] = std::get<0>(triangle);
    points[1] = std::get<1>(triangle);
    points[2] = std::get<2>(triangle);
    for (int i = 0; i < 3; i++) {
      auto it = pointsFound.find(points[i]);
      if (it == pointsFound.end()) { // if the point wasn't found yet
        this->points.push_back(points[i]); //add point to points vector
        pointsFound[points[i]] = this->points.size() - 1; //add to the map where it is stored
        pos[i] = pointsFound[points[i]];
      } else {
        pos[i] = it->second;
      }
    }
    //save the triangle as the positions of the points in the vector of points
    this->trianglesByPos.push_back({pos[0], pos[1], pos[2]});
  }
}

Shape::Shape(std::string filePath) {
  std::ifstream file(filePath); //open the file

  if (!file) {

    std::stringstream exception_message;
    exception_message << "XMLParser@model: The file '" << filePath
                      << "' does not exist.";
    throw InvalidXMLStructure(exception_message.str());
  }

  int n;
  file >> n; //read number of points
  for (int i = 0; i < n; i++) {
    float x, y, z;
    file >> x >> y >> z; // read x,y,z for each point
    this->points.push_back({x, y, z}); // add the point to vector of points
  }
  file >> n; // read number of triangles
  for (int i = 0; i < n; i++) {
    int t1, t2, t3;
    file >> t1 >> t2 >> t3; // read the position of each point of the triangle in the vector of points
    this->trianglesByPos.push_back({t1, t2, t3}); // add the triangle to the vector of trianglesByPos
  }
  file.close();//close the file
}

Shape::Shape(const Shape& shape) :
  points(shape.points),
  trianglesByPos(shape.trianglesByPos)
{}

Shape::Shape(Shape&& shape) :
  points(std::move(shape.points)),
  trianglesByPos(std::move(shape.trianglesByPos)),
  vbo_addr(shape.vbo_addr),
  vertexCount(shape.vertexCount)
{}


Shape::~Shape() {
  if(this->vbo_addr != 0)
    glDeleteBuffers(1, &this->vbo_addr);
}


Shape& Shape::operator=(const Shape& shape) {
  this->points = shape.points;
  this->trianglesByPos = shape.trianglesByPos;

  return *this;
}


Shape& Shape::operator=(Shape&& shape) {
  this->points = std::move(shape.points);
  this->trianglesByPos = std::move(shape.trianglesByPos);
  this->vbo_addr = shape.vbo_addr;
  this->vertexCount = shape.vertexCount;

  return *this;
}


void push_tuple(std::vector<float>& v, std::tuple<float,float,float> t) {
  v.push_back(std::get<0>(t));
  v.push_back(std::get<1>(t));
  v.push_back(std::get<2>(t));
}

void Shape::initialize() {
  std::vector<float> p;
	
  //TODO: use vbo indexes
  for (TriangleByPosition t : this->trianglesByPos) {
    push_tuple(p, this->points[std::get<0>(t)]);
    push_tuple(p, this->points[std::get<1>(t)]);
    push_tuple(p, this->points[std::get<2>(t)]);
  }

	glGenBuffers(1, &this->vbo_addr);
	this->vertexCount = p.size() / 3;

	// copiar o vector para a memória gráfica
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_addr);
	glBufferData(
		GL_ARRAY_BUFFER, // tipo do buffer, só é relevante na altura do desenho
		sizeof(float) * p.size(), // tamanho do vector em bytes
		p.data(), // os dados do array associado ao vector
		GL_STATIC_DRAW // indicativo da utilização (estático e para desenho)
	);
}

void Shape::draw() {
  /*
  for (TriangleByPosition &triangle : this->trianglesByPos) {
    Point p1 = this->points[std::get<0>(triangle)];
    Point p2 = this->points[std::get<1>(triangle)];
    Point p3 = this->points[std::get<2>(triangle)];

    glBegin(GL_TRIANGLES);

    glVertex3f(std::get<0>(p1), std::get<1>(p1), std::get<2>(p1));
    glVertex3f(std::get<0>(p2), std::get<1>(p2), std::get<2>(p2));
    glVertex3f(std::get<0>(p3), std::get<1>(p3), std::get<2>(p3));
    glEnd();
  }
  */

  if (vbo_addr == 0)
    throw std::runtime_error("Attept to draw uninitialized shape");

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_addr);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
}

bool Shape::exportToFile(std::string filePath) {
  std::ofstream file(filePath);
  file << this->points.size() << std::endl;//write the number of points
  for (Point p : this->points)
    file << std::get<0>(p) << " " << std::get<1>(p) << " " << std::get<2>(p)
         << std::endl; // for each point write its x,y,z coordenates
  file << this->trianglesByPos.size() << std::endl;//write the number of triangles
  for (TriangleByPosition pos : this->trianglesByPos)
    file << std::get<0>(pos) << " " << std::get<1>(pos) << " "
         << std::get<2>(pos) << std::endl; // for each triangle write the position in the points vector of the points that compose the triangle
  file.close();
  return true;
}
