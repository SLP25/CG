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


Shape::Shape() : vbo_points(0), vbo_normals(0), vbo_textures(0) {}

Shape::Shape(const std::vector<Triangle>& triangles) : vbo_points(0), vbo_normals(0), vbo_textures(0) {
  //points found in the vector of triangles and the position they will be stored in the points vector
  std::map<std::pair<Point,Vector>, int> verticesFound; 

  for (const Triangle& triangle : triangles) {
    Vector normal = getNormal(triangle);
    std::pair<Point,Vector> vertices[3] = {
      { std::get<0>(triangle), normal },
      { std::get<1>(triangle), normal },
      { std::get<2>(triangle), normal }
    };
    int pos[3];

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

  this->boundingBox = BoundingBox(this->points);
}

Shape::Shape(std::vector<Point> points, std::vector<Vector> normals, std::vector<Point2D> textures, std::vector<TriangleByPosition> trianglesByPos) :
  points(points),
  normals(normals),
  textures(textures),
  boundingBox(points),
  vbo_points(0),
  vbo_normals(0),
  vbo_textures(0),
  trianglesByPos(trianglesByPos)
{}

Shape::Shape(const std::vector<Triangle>& triangles, const std::vector<Vector>& normals, const std::vector<Point2D>& textureCoordinates) :
  vbo_points(0),
  vbo_normals(0),
  vbo_textures(0)
{
  std::map<std::tuple<Point,Vector,Point2D>, int> verticesFound; 

  for (int i = 0; i < (int)triangles.size(); i++) {
    Triangle triangle = triangles[i];
    std::tuple<Point,Vector,Point2D> vertices[3] = {
      { std::get<0>(triangle), normals[3 * i], textureCoordinates[3 * i] },
      { std::get<1>(triangle), normals[3 * i + 1], textureCoordinates[3 * i + 1] },
      { std::get<2>(triangle), normals[3 * i + 2], textureCoordinates[3 * i + 2] }
    };
    int pos[3];

    for (int i = 0; i < 3; i++) {
      auto it = verticesFound.find(vertices[i]);
      if (it == verticesFound.end()) { // if the point wasn't found yet
        this->points.push_back(std::get<0>(vertices[i]));
        this->normals.push_back(std::get<1>(vertices[i]));
        this->textures.push_back(std::get<2>(vertices[i]));
        pos[i] = verticesFound[vertices[i]] = this->points.size() - 1; //add to the map where it is stored
      } else {
        pos[i] = it->second;
      }
    }
    //save the triangle as the positions of the points in the vector of points
    this->trianglesByPos.push_back({pos[0], pos[1], pos[2]});
  }

  this->boundingBox = BoundingBox(this->points);
}

Shape::Shape(const std::vector<Triangle>& triangles, const std::map<Point, Point2D>& textureCoordinates) :
  vbo_points(0),
  vbo_normals(0),
  vbo_textures(0)
{
  std::map<std::tuple<Point,Vector,Point2D>, int> verticesFound; 

  for (const Triangle &triangle : triangles) {
    Vector normal = getNormal(triangle);
    std::tuple<Point,Vector,Point2D> vertices[3] = {
      { std::get<0>(triangle), normal, textureCoordinates.at(std::get<0>(triangle)) },
      { std::get<1>(triangle), normal, textureCoordinates.at(std::get<1>(triangle)) },
      { std::get<2>(triangle), normal, textureCoordinates.at(std::get<2>(triangle)) }
    };
    int pos[3];

    for (int i = 0; i < 3; i++) {
      auto it = verticesFound.find(vertices[i]);
      if (it == verticesFound.end()) { // if the point wasn't found yet
        this->points.push_back(std::get<0>(vertices[i]));
        this->normals.push_back(std::get<1>(vertices[i]));
        this->textures.push_back(std::get<2>(vertices[i]));
        pos[i] = verticesFound[vertices[i]] = this->points.size() - 1; //add to the map where it is stored
      } else {
        pos[i] = it->second;
      }
    }
    //save the triangle as the positions of the points in the vector of points
    this->trianglesByPos.push_back({pos[0], pos[1], pos[2]});
  }

  this->boundingBox = BoundingBox(this->points);
}

Shape::Shape(std::string filePath) : vbo_points(0), vbo_normals(0), vbo_textures(0) {
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
  //number of normals is the same as number of points
  for (int i = 0; i < n; i++) {
    float x, y, z;
    file >> x >> y >> z; // read x,y,z for each point
    this->normals.push_back({x, y, z}); // add the vector to vector of normals
  }
  for (int i = 0; i < n; i++) {
    float u, v;
    file >> u >> v;
    this->textures.push_back({u, v});
  }

  file >> n; // read number of triangles
  for (int i = 0; i < n; i++) {
    int t1, t2, t3;
    file >> t1 >> t2 >> t3; // read the position of each point of the triangle in the vector of points
    this->trianglesByPos.push_back({t1, t2, t3}); // add the triangle to the vector of trianglesByPos
  }

  file.close();//close the file
  this->boundingBox = BoundingBox(this->points);
}

Shape::Shape(const Shape& shape) :
  points(shape.points),
  normals(shape.normals),
  textures(shape.textures),
  boundingBox(shape.boundingBox),
  vbo_points(0),
  vbo_normals(0),
  vbo_textures(0),
  trianglesByPos(shape.trianglesByPos)
{}

Shape::Shape(Shape&& shape) :
  points(std::move(shape.points)),
  normals(std::move(shape.normals)),
  textures(std::move(shape.textures)),
  boundingBox(shape.boundingBox),
  vbo_points(shape.vbo_points),
  vbo_normals(shape.vbo_normals),
  vbo_textures(shape.vbo_textures),
  trianglesByPos(std::move(shape.trianglesByPos))
{}


Shape::~Shape() {
  if(this->vbo_points != 0)
    glDeleteBuffers(1, &this->vbo_points);

  if (this->vbo_normals != 0)
    glDeleteBuffers(1, &this->vbo_normals);

  if (this->vbo_textures != 0)
    glDeleteBuffers(1, &this->vbo_textures);
}


Shape& Shape::operator=(const Shape& shape) {
  this->points = shape.points;
  this->normals = shape.normals;
  this->textures = shape.textures;
  this->boundingBox = shape.boundingBox;

  if (this->vbo_points != 0) {
    glDeleteBuffers(1, &this->vbo_points);
    this->vbo_points = 0;
  }

  if (this->vbo_normals != 0) {
    glDeleteBuffers(1, &this->vbo_normals);
    this->vbo_normals = 0;
  }

  if (this->vbo_textures != 0) {
    glDeleteBuffers(1, &this->vbo_textures);
    this->vbo_textures = 0;
  }

  this->trianglesByPos = shape.trianglesByPos;
  return *this;
}


Shape& Shape::operator=(Shape&& shape) {
  this->points = std::move(shape.points);
  this->normals = std::move(shape.normals);
  this->textures = std::move(shape.textures);
  this->boundingBox = shape.boundingBox;

  if (this->vbo_points != 0)
    glDeleteBuffers(1, &this->vbo_points);

  if (this->vbo_normals != 0)
    glDeleteBuffers(1, &this->vbo_normals);

  if (this->vbo_textures != 0)
    glDeleteBuffers(1, &this->vbo_textures);

  this->vbo_points = shape.vbo_points;
  this->vbo_normals = shape.vbo_normals;
  this->vbo_textures = shape.vbo_textures;
  this->trianglesByPos = std::move(shape.trianglesByPos);
  return *this;
}


void push_tuple(std::vector<float>& v, std::tuple<float,float,float> t) {
  v.push_back(std::get<0>(t));
  v.push_back(std::get<1>(t));
  v.push_back(std::get<2>(t));
}

void push_tuple(std::vector<float>& v, std::tuple<float,float> t) {
  v.push_back(std::get<0>(t));
  v.push_back(std::get<1>(t));
}

void Shape::initialize() {
  std::vector<float> p, n, t;

  //TODO: use vbo indexes
  for (TriangleByPosition tr : this->trianglesByPos) {
    push_tuple(p, this->points[std::get<0>(tr)]);
    push_tuple(p, this->points[std::get<1>(tr)]);
    push_tuple(p, this->points[std::get<2>(tr)]);

    push_tuple(n, this->normals[std::get<0>(tr)]);
    push_tuple(n, this->normals[std::get<1>(tr)]);
    push_tuple(n, this->normals[std::get<2>(tr)]);

    push_tuple(t, this->textures[std::get<0>(tr)]);
    push_tuple(t, this->textures[std::get<1>(tr)]);
    push_tuple(t, this->textures[std::get<2>(tr)]);
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

  glGenBuffers(1, &this->vbo_textures);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_textures);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * t.size(), t.data(), GL_STATIC_DRAW);
}

BoundingBox Shape::getBoundingBox() {
  return boundingBox;
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

  if (vbo_points == 0 || vbo_normals == 0 || vbo_textures == 0)
    throw std::runtime_error("Attept to draw uninitialized shape");

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_points);
	glVertexPointer(3, GL_FLOAT, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normals);
  glNormalPointer(GL_FLOAT, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_textures);
  glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, this->trianglesByPos.size() * 3);
  //glDrawElements(GL_TRIANGLES, this->points.size(), GL_UNSIGNED_INT, indices);
}

bool Shape::exportToFile(std::string filePath) {
  std::ofstream file(filePath);
  int n = this->points.size();
  file << n << std::endl;//write the number of points

  for (Point p : this->points) {
    file << std::get<0>(p) << " " << std::get<1>(p) << " " << std::get<2>(p)
         << std::endl; // for each point write its x,y,z coordenates
  }

  for (Vector n : this->normals)
    file << std::get<0>(n) << " " << std::get<1>(n) << " " << std::get<2>(n)
         << std::endl; // for each normal write its x,y,z coordenates

  for (Point2D t : this->textures)
    file << std::get<0>(t) << " " << std::get<1>(t)
         << std::endl; // for each point write its u,v coordenates
    
  file << this->trianglesByPos.size() << std::endl;//write the number of triangles
  for (TriangleByPosition pos : this->trianglesByPos)
    file << std::get<0>(pos) << " " << std::get<1>(pos) << " "
         << std::get<2>(pos) << std::endl; // for each triangle write the position in the points vector of the points that compose the triangle
  
  file.close();
  
  return true;
}
