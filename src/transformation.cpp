#include "glut.hpp"

#include "utils.hpp"
#include "parser.hpp"
#include "transformation.hpp"
#include <iostream>
#include <cmath>

std::unique_ptr<Transformation> Transformation::parse(XMLParser parser) {
  return dynamicParser<Transformation,Translation,Rotation,Scale,CatmullRom>::parse(parser);
}


bool Translation::accepts(XMLParser parser) {
  float x;
  return parser.name() == "translate" && parser.get_opt_attr("x", x);
}

Translation::Translation(XMLParser parser) {
  parser.validate_node({});
  parser.validate_attrs({"x", "y", "z"});

  this->x = parser.get_attr<float>("x");
  this->y = parser.get_attr<float>("y");
  this->z = parser.get_attr<float>("z");
}

Transformation::~Transformation() {}

Transformation* Translation::clone() {
  return new Translation(*this);
}

void Translation::apply() { glTranslatef(this->x, this->y, this->z); }


bool Rotation::accepts(XMLParser parser) {
  return parser.name() == "rotate";
}

Rotation::Rotation(XMLParser parser) : time(0.0f), angle(0.0f) {

  parser.validate_node({});
  parser.validate_attrs({"x", "y", "z", "angle", "time"});

  this->x = parser.get_attr<float>("x");
  this->y = parser.get_attr<float>("y");
  this->z = parser.get_attr<float>("z");

  bool a = parser.get_opt_attr<float>("angle", this->angle);
  bool b = parser.get_opt_attr<float>("time", this->time);

  if (!a && !b)
    throw InvalidXMLStructure("Rotate must define either angle or time (or both)");
}

Transformation* Rotation::clone() {
  return new Rotation(*this);
}

void Rotation::apply() {
  float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
  float w = this->time != 0 ? 360 / this->time : 0;

  glRotatef(this->angle + t * w, this->x, this->y, this->z);
}


bool Scale::accepts(XMLParser parser) {
  return parser.name() == "scale";
}

Scale::Scale(XMLParser parser) {
  parser.validate_node({});
  float s;

  if (parser.get_opt_attr<float>("s", s))
  {
    parser.validate_attrs({"s"});
    this->x = this->y = this->z = s;
  }
  else
  {
    parser.validate_attrs({"x", "y", "z"});
    this->x = parser.get_attr<float>("x");
    this->y = parser.get_attr<float>("y");
    this->z = parser.get_attr<float>("z");
  }
}

Transformation* Scale::clone() {
  return new Scale(*this);
}

void Scale::apply() { glScalef(this->x, this->y, this->z); }


bool CatmullRom::accepts(XMLParser parser) {
  float time;
  return parser.name() == "translate" && parser.get_opt_attr<float>("time", time);
}

CatmullRom::CatmullRom(XMLParser parser) {
  parser.validate_node({"point"});
  parser.validate_attrs({"time", "align", "trace"});

  this->time = parser.get_attr<float>("time");

  std::string aux = "false";
  parser.get_opt_attr<std::string>("align", aux);
  this->align = parseBool(aux);

  aux = "false";
  parser.get_opt_attr<std::string>("trace", aux);
  this->trace = parseBool(aux);

  std::vector<XMLParser> nodes = parser.get_nodes("point");

  if (nodes.size() < 4)
    throw InvalidXMLStructure("At least 4 points are needed for Catmull-Rom curve");

  for (XMLParser& n : nodes)
    this->points.push_back(n.as_tuple<float,float,float>({"x", "y", "z"}));
}

Transformation* CatmullRom::clone() {
  return new CatmullRom(*this);
}


void CatmullRom::apply()
{
  if (this->trace)
    this->draw();

  Point pos;
  Vector deriv;
	getGlobalCatmullRomPoint(glutGet(GLUT_ELAPSED_TIME) / 1000.0f, pos, deriv);

  glTranslatef(std::get<0>(pos), std::get<1>(pos), std::get<2>(pos));

  if (this->align) {
    deriv = normalize(deriv);
    Vector z = normalize(crossProduct(deriv, this->y_0));
    Vector y = normalize(crossProduct(z, deriv));

    float m[16] = {
      std::get<0>(deriv), std::get<1>(deriv), std::get<2>(deriv), 0,
      std::get<0>(y),     std::get<1>(y),     std::get<2>(y),     0,
      std::get<0>(z),     std::get<1>(z),     std::get<2>(z),     0,
      0,                  0,                  0,                  1
    };

    glMultMatrixf(m);
    this->y_0 = y;
  }
}

void CatmullRom::draw() {
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINE_LOOP);

	for (float t = 0; t <= this->time; t += 0.1) {
		Point pos;
    Vector deriv;
		this->getGlobalCatmullRomPoint(t, pos, deriv);
		glVertex3f(std::get<0>(pos), std::get<1>(pos), std::get<2>(pos));
	}

	glEnd();
}


void getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3, Point& pos, Vector& deriv) {
  // catmull-rom matrix
	float M[16] = {	-0.5f,  1.5f,   -1.5f,  0.5f,
						      1.0f,   -2.5f,  2.0f,   -0.5f,
						      -0.5f,  0.0f,   0.5f,   0.0f,
						      0.0f,   1.0f,   0.0f,   0.0f };

  float P[16] = { std::get<0>(p0), std::get<1>(p0), std::get<2>(p0), 1,
                  std::get<0>(p1), std::get<1>(p1), std::get<2>(p1), 1,
                  std::get<0>(p2), std::get<1>(p2), std::get<2>(p2), 1,
                  std::get<0>(p3), std::get<1>(p3), std::get<2>(p3), 1 };
	
	float T[4] = { t*t*t, t*t, t, 1 };
	float dT[4] = { 3*t*t, 2*t, 1, 0 };

  float A[16];  //TODO: optimization: store matrix (is the same for the same segment)
  matrixProd(4, 4, 4, M, P, A);

  float _pos[4], _deriv[4];
  matrixProd(4, 1, 4, T, A, _pos);
  matrixProd(4, 1, 4, dT, A, _deriv);

  pos = { _pos[0], _pos[1], _pos[2] };
  deriv = { _deriv[0], _deriv[1], _deriv[2] };
}

void CatmullRom::getGlobalCatmullRomPoint(float gt, Point& pos, Vector& deriv) {
  float t = gt / this->time * this->points.size(); // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within the segment

	// indices store the points
	int indices[4]; 
	indices[0] = (index + this->points.size()-1)%this->points.size();	
	indices[1] = (indices[0]+1)%this->points.size();
	indices[2] = (indices[1]+1)%this->points.size(); 
	indices[3] = (indices[2]+1)%this->points.size();

	getCatmullRomPoint(t, this->points[indices[0]], this->points[indices[1]],
        this->points[indices[2]], this->points[indices[3]], pos, deriv);
}