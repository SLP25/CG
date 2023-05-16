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


Shape::Shape() : vbo_points(0), vbo_normals(0) {}

Shape::Shape(std::vector<Triangle> triangles) : vbo_points(0), vbo_normals(0) {
  //points found in the vector of triangles and the position they will be stored in the points vector
  std::map<std::pair<Point,Vector>, int> verticesFound; 
  std::pair<Point,Vector> vertices[3];
  int pos[3];
  
  for (Triangle &triangle : triangles) {
    Vector normal = getNormal(triangle);

    vertices[0] = { std::get<0>(triangle), normal };
    vertices[1] = { std::get<1>(triangle), normal };
    vertices[2] = { std::get<2>(triangle), normal };

    for (int i = 0; i < 3; i++) {
      auto it = verticesFound.find(vertices[i]);
      if (it == verticesFound.end()) { // if the point wasn't found yet
        this->points.push_back(vertices[i].first);
        this->normals.push_back(vertices[i].second);
        pos[i] = verticesFound[vertices[i]] = this->points.size() - 1; //add to the map where it is stored
      } else {
        pos[i] = it->second;
      }
    }
    //save the triangle as the positions of the points in the vector of points
    this->trianglesByPos.push_back({pos[0], pos[1], pos[2]});
  }

}

Shape::Shape(std::vector<Point> points, std::vector<Vector> normals, std::vector<TriangleByPosition> trianglesByPos) :
  points(points),
  normals(normals),
  vbo_points(0),
  vbo_normals(0),
  trianglesByPos(trianglesByPos)
{}

Shape::Shape(std::vector<Triangle> triangles, std::map<Point, Point> textureCoordinates) : vbo_points(0), vbo_normals(0), vbo_addr(0) {
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
  this->textureMapping = std::move(textureCoordinates);
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
    float x, y, z, u, v;
    file >> x >> y >> z >> u >> v; // read x,y,z for each point
    this->points.push_back({x, y, z}); // add the point to vector of points
    if(u != -1)
      this->textureMapping[{x,y,z}] = {u,v,0};
  }
  //number of normals is the same as number of points
  for (int i = 0; i < n; i++) {
    float x, y, z;
    file >> x >> y >> z; // read x,y,z for each point
    this->normals.push_back({x, y, z}); // add the vector to vector of normals
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
  points(std::move(shape.points)),
  normals(std::move(shape.normals)),
  vbo_points(shape.vbo_points),
  vbo_normals(shape.vbo_normals),
  trianglesByPos(std::move(shape.trianglesByPos)),
  textureMapping(shape.textureMapping),
  vertexCount(shape.vertexCount)
{}

Shape::Shape(Shape&& shape) :
  points(std::move(shape.points)),
  normals(std::move(shape.normals)),
  vbo_points(shape.vbo_points),
  vbo_normals(shape.vbo_normals),
  trianglesByPos(std::move(shape.trianglesByPos)),
  textureMapping(shape.textureMapping),
  vertexCount(shape.vertexCount)
{}


Shape::~Shape() {
  if(this->vbo_points != 0)
    glDeleteBuffers(1, &this->vbo_points);

  if (this->vbo_normals != 0)
    glDeleteBuffers(1, &this->vbo_normals);
}


Shape& Shape::operator=(const Shape& shape) {
  this->points = shape.points;
  this->normals = shape.normals;
  this->trianglesByPos = shape.trianglesByPos;
  this->vbo_addr = shape.vbo_addr;
  this->vertexCount = shape.vertexCount;
  this->textureMapping = shape.textureMapping;

  if (this->vbo_points != 0) {
    glDeleteBuffers(1, &this->vbo_points);
    this->vbo_points = 0;
  }

  if (this->vbo_normals != 0) {
    glDeleteBuffers(1, &this->vbo_normals);
    this->vbo_normals = 0;
  }

  return *this;
}


Shape& Shape::operator=(Shape&& shape) {
  this->points = std::move(shape.points);
  this->normals = std::move(shape.normals);
  this->trianglesByPos = std::move(shape.trianglesByPos);

  if (this->vbo_points != 0)
    glDeleteBuffers(1, &this->vbo_points);

  if (this->vbo_normals != 0)
    glDeleteBuffers(1, &this->vbo_normals);

  this->vbo_points = shape.vbo_points;
  this->vbo_normals = shape.vbo_normals;
  this->vbo_addr = shape.vbo_addr;
  this->vertexCount = shape.vertexCount;
  this->textureMapping = shape.textureMapping;

  return *this;
}


void push_tuple(std::vector<float>& v, std::tuple<float,float,float> t) {
  v.push_back(std::get<0>(t));
  v.push_back(std::get<1>(t));
  v.push_back(std::get<2>(t));
}

void Shape::initialize() {
  std::vector<float> p, n;

  //TODO: use vbo indexes
  for (TriangleByPosition t : this->trianglesByPos) {
    push_tuple(p, this->points[std::get<0>(t)]);
    push_tuple(p, this->points[std::get<1>(t)]);
    push_tuple(p, this->points[std::get<2>(t)]);
  }

  for (TriangleByPosition t : this->trianglesByPos) {
    push_tuple(n, this->normals[std::get<0>(t)]);
    push_tuple(n, this->normals[std::get<1>(t)]);
    push_tuple(n, this->normals[std::get<2>(t)]);
  }

  glGenBuffers(1, &this->vbo_points);

	// copiar o vector para a memória gráfica
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_points);
	glBufferData(
		GL_ARRAY_BUFFER, // tipo do buffer, só é relevante na altura do desenho
		sizeof(float) * p.size(), // tamanho do vector em bytes
		p.data(), // os dados do array associado ao vector
		GL_STATIC_DRAW // indicativo da utilização (estático e para desenho)
	);

  glGenBuffers(1, &this->vbo_normals);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n.size(), n.data(), GL_STATIC_DRAW);
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

  if (vbo_points == 0 || vbo_normals == 0)
    throw std::runtime_error("Attept to draw uninitialized shape");

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_points);
	glVertexPointer(3, GL_FLOAT, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normals);
  glNormalPointer(GL_FLOAT, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, this->trianglesByPos.size() * 3);
  //glDrawElements(GL_TRIANGLES, this->points.size(), GL_UNSIGNED_INT, indices);
}

bool Shape::exportToFile(std::string filePath) {
  std::ofstream file(filePath);
  file << this->points.size() << std::endl;//write the number of points

  for (Point p : this->points) {
    file << std::get<0>(p) << " " << std::get<1>(p) << " " << std::get<2>(p);

    if (this->textureMapping.find(p) != this->textureMapping.end()) 
      file << " " << std::get<0>(this->textureMapping[p]) << " " << std::get<1>(this->textureMapping[p]);
    else
      file << " -1 -1";

    file << std::endl; // for each point write its x,y,z coordenates
  }

  for (Vector n : this->normals)
    file << std::get<0>(n) << " " << std::get<1>(n) << " " << std::get<2>(n)
         << std::endl; // for each point write its x,y,z coordenates
    
  file << this->trianglesByPos.size() << std::endl;//write the number of triangles
  for (TriangleByPosition pos : this->trianglesByPos)
    file << std::get<0>(pos) << " " << std::get<1>(pos) << " "
         << std::get<2>(pos) << std::endl; // for each triangle write the position in the points vector of the points that compose the triangle
  
  file.close();
  
  return true;
}
