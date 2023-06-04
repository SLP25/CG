#include "utils.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

Point average(std::initializer_list<Point> points) {
  /**
  * @brief Calculates the average of the points by adding their
  * components and dividing by the number of points
  * 
  * @param points the list of points to calculate the average
  * @return Point The average point
  */
  return average(points.begin(), points.end());
}

float angle(Vector u,Vector v){
  /**
   * @brief Calculates the angle between two vectors
   * 
   * @param u the first vector
   * @param v the second vector
   * 
   * @return the angle between the two vectors in degrees
   */
  return acos(u * v / (length(u) * length(v)));
}

Vector projectToVector(Vector u, Vector v) {
  /**
   * @brief calculates the projection of a vector onto another vector
   * 
   * @param u the vector to project to
   * @param v the vector to be projected
   * 
   * @return the projection of vector v onto vector u
   * 
   */
  return (u * v / square_length(u)) * u;
}

Vector projectToPlane(Vector n, Vector v) {
  /**
   * @brief calculates the projection of a vector onto a plane
   * 
   * @param u the normal vector of the plane to project to
   * @param v the vector to be projected
   * 
   * @return the projection of vector v onto the plane with normal vector u
   * 
   */
  return v - projectToVector(n, v);
}


Vector rotate(Vector around,Vector v,float angle){
  /**
   * @brief calculate the resulting vector of rotating a vector around another vector by a given angle
   * 
   * @param around the vector to rotate around
   * @param v the vector to rotate
   * @param angle the angle to rotate by in degrees
   * 
   * @return the vector resulting of rotating v angle degrees around around
   */
  Vector axis = normalize(around);
  float cossine = cos(angle);
  float sinne = sin(angle);
  return (v * cossine)
        + (sinne * (axis ^ v))
        + ((1 - cossine) * (v * axis) * axis);
}

Vector getNormal(Triangle triangle) {
  Vector v1 = std::get<2>(triangle) - std::get<1>(triangle);
  Vector v2 = std::get<0>(triangle) - std::get<1>(triangle);
  Vector cross = v1 ^ v2;
  return cross == zero() ? zero() : normalize(cross);
}

std::tuple<float, float, float> parseHexColor(std::string colorHex) {
  /**
   * @brief Converts a string containing a hex representation of a color into rgb
   * @return A tuple with values of {red,green,blue}
   */
  unsigned int rgb[3];
  for(int i = 0; i < 3; i++) {
    std::stringstream ss;
    ss << std::hex << colorHex.substr(1 + i * 2,2);
    ss >> rgb[i];
  }

  return {(float)rgb[0] / 255.0f, (float)rgb[1] / 255.0f, (float)rgb[2] / 255.0f};
}

bool parseBool(std::string str) {
  for (char& c : str)
    c = std::tolower(c);

  if (str == "true")
    return true;

  if (str == "false")
    return false;

  throw InvalidXMLStructure("Input '" + str + "' isn't a boolean");
}


BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox(const std::vector<Point>& points) {
  //Calculate as an AABB
  float minX, maxX, minY, maxY, minZ, maxZ;
  minX = maxX = std::get<0>(points.at(0));
  minY = maxY = std::get<1>(points.at(0));
  minZ = maxZ = std::get<2>(points.at(0));

  for (unsigned int i = 1; i < points.size(); i++) {
    float x = std::get<0>(points.at(i));
    float y = std::get<1>(points.at(i));
    float z = std::get<2>(points.at(i));

    if (x < minX) minX = x; else if (x > maxX) maxX = x;
    if (y < minY) minY = y; else if (y > maxY) maxY = y;
    if (z < minZ) minZ = z; else if (z > maxZ) maxZ = z;
  }

  for (float x : { minX, maxX })
    for (float y : { minY, maxY })
      for (float z : { minZ, maxZ })
        corners.push_back({ x, y, z });
}

BoundingBox::BoundingBox(const BoundingBox& bb) :
  corners(bb.corners)
{}

BoundingBox& BoundingBox::operator =(const BoundingBox& bb) {
  this->corners = bb.corners;
  return *this;
}

void BoundingBox::transform(float modelview[16]) {
  for (Point& p : corners) {
    float input[4] = { GET_ALL(p), 1 }, output[4];
    matrixProd(1, 4, 4, modelview, input, output);

    p = { output[0], output[1], output[2] };
  }
}

bool BoundingBox::isForward(Plane plane) {
  for (Point& p : corners)
    if (p * plane.normal >= plane.displacement)
      return true;

  return false;
}


Frustum::Frustum(Point position, Vector lookAtVector, Vector up, float near, float far, float fov, float ratio) {
  lookAtVector = normalize(lookAtVector);
  up = normalize(up);

  Point centerNear = position + lookAtVector * near;
  Point centerFar = position + lookAtVector * far;

  this->near = Plane(centerNear, lookAtVector);
  this->far = Plane(centerFar, -lookAtVector);

  float halfHeightNear = near * tan(fov * M_PI / 360);
  float halfWidthNear = halfHeightNear * ratio;

  Vector right = normalize(lookAtVector ^ up);
  Point upRightNear = centerNear + up * halfHeightNear + right * halfWidthNear;
  Vector upRightOut = upRightNear - position;
  Point downLeftNear = centerNear - up * halfHeightNear - right * halfWidthNear;
  Vector downLeftOut = downLeftNear - position;
  
  this->up = Plane(upRightNear, normalize(upRightOut ^ right));
  this->down = Plane(downLeftNear, normalize(right ^ downLeftOut));
  this->left = Plane(downLeftNear, normalize(downLeftOut ^ up));
  this->right = Plane(upRightNear, normalize(up ^ upRightOut));
}

bool Frustum::contains(BoundingBox boundingBox) const {
  for (Plane p : { up, down, left, right, near, far })
    if (!boundingBox.isForward(p))
      return false;
  
  return true;
}