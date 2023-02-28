#pragma once

#include "config.hpp"

#define PI 3.141592654f

Vector zero();
Vector inverse(Vector v);
Vector scale(float n, Vector v);
float length(Vector v);
float square_length(Vector v);
Vector normalize(Vector v);
Vector difference(Point p1, Point p2);
Point addVector(Point p, Vector v);
float dotProduct(Vector u, Vector v);
Vector crossProduct(Vector u, Vector v);
Vector projectToVector(Vector u, Vector v);
Vector projectToPlane(Vector n, Vector v);