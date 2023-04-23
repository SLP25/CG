/**
 * @file shapegenerator.cpp
 *
 * @brief File implementing functions used to generate the different shapes
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include "shapegenerator.hpp"
#include "utils.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>



// displacement is a fraction of the angle of slicing. 0.5 displacement means
// the circle is offset by PI/slices rad
std::vector<Point> generateCircle(Point center, float radius, int slices,
                                  float displacement) {
  std::vector<Point> ans;

  for (int i = 0; i < slices; i++) {
    float angle = 2 * M_PI * (i + displacement) / slices;
    Vector v = {radius * cos(angle), 0, radius * sin(angle)};
    ans.push_back(addVector(center, v));
  }

  return ans;
}

void generateSquare(Point p1, Point p2, Point p3, Point p4,
                    std::vector<Triangle> &triangles) {
  triangles.push_back({p1, p2, p3});
  triangles.push_back({p1, p3, p4});
}

void generatePolygon(std::vector<Point> vertices,
                     std::vector<Triangle> &triangles) {
  Point center = average(vertices.begin(), vertices.end());

  for (int i = 0; i < (int)vertices.size(); i++)
    triangles.push_back({vertices.at(i), vertices.at((i + 1) % vertices.size()), center});
}

std::unique_ptr<Shape> generatePlane(float length, int divisions) {
  /*

  In order to generate a plane with length l and d divisions, we do the
  following:

  1. Compute all the relevant points in the plane
  2. Create a mapping between these points and their indices in the
  vector that stores them. This will be useful when computing the faces
  3. Compute the vertices belonging to each face. Use the reverse mapping
  to store the indices of all vertices of a face. They must be generated
  in counter-clockwise fashion

  */
  std::vector<Triangle> triangles;

  float mid = length / 2.0;
  float step = length / divisions;

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({-mid + i * step, 0, -mid + j * step},
                     {-mid + i * step, 0, -mid + (j + 1) * step},
                     {-mid + (i + 1) * step, 0, -mid + (j + 1) * step},
                     {-mid + (i + 1) * step, 0, -mid + j * step}, triangles);
    }
  }

  return std::make_unique<Shape>(triangles);
}

std::unique_ptr<Shape> generateCube(float length, int divisions) {
  std::vector<Triangle> triangles;

  float mid = length / 2.0;
  float step = length / divisions;

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({-mid, -mid + i * step, -mid + j * step},
                     {-mid, -mid + i * step, -mid + (j + 1) * step},
                     {-mid, -mid + (i + 1) * step, -mid + (j + 1) * step},
                     {-mid, -mid + (i + 1) * step, -mid + j * step}, triangles);
    }
  }

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({mid, -mid + (i + 1) * step, -mid + j * step},
                     {mid, -mid + (i + 1) * step, -mid + (j + 1) * step},
                     {mid, -mid + i * step, -mid + (j + 1) * step},
                     {mid, -mid + i * step, -mid + j * step}, triangles);
    }
  }

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({-mid + (i + 1) * step, -mid, -mid + j * step},
                     {-mid + (i + 1) * step, -mid, -mid + (j + 1) * step},
                     {-mid + i * step, -mid, -mid + (j + 1) * step},
                     {-mid + i * step, -mid, -mid + j * step}, triangles);
    }
  }

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({-mid + i * step, mid, -mid + j * step},
                     {-mid + i * step, mid, -mid + (j + 1) * step},
                     {-mid + (i + 1) * step, mid, -mid + (j + 1) * step},
                     {-mid + (i + 1) * step, mid, -mid + j * step}, triangles);
    }
  }

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({-mid + i * step, -mid + j * step, -mid},
                     {-mid + i * step, -mid + (j + 1) * step, -mid},
                     {-mid + (i + 1) * step, -mid + (j + 1) * step, -mid},
                     {-mid + (i + 1) * step, -mid + j * step, -mid}, triangles);
    }
  }

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({-mid + (i + 1) * step, -mid + j * step, mid},
                     {-mid + (i + 1) * step, -mid + (j + 1) * step, mid},
                     {-mid + i * step, -mid + (j + 1) * step, mid},
                     {-mid + i * step, -mid + j * step, mid}, triangles);
    }
  }

  return std::make_unique<Shape>(triangles);
}

std::unique_ptr<Shape> generateCylinder(float radius, float height,
                                        int slices) {
  std::vector<Point> top = generateCircle({0, height, 0}, radius, slices, 0);
  std::vector<Point> bottom = generateCircle({0, 0, 0}, radius, slices, 0.5);
  std::vector<Triangle> ans;

  for (int i = 0; i < slices; i++) {
    int i2 = (i + 1) % slices;
    ans.push_back({top[i], top[i2], bottom[i]});
    ans.push_back({bottom[i2], bottom[i], top[i2]});
  }

  reverse(top.begin(), top.end());
  generatePolygon(top, ans);
  generatePolygon(bottom, ans);

  return std::make_unique<Shape>(ans);
}

std::unique_ptr<Shape> generateCone(float radius, float height, int slices,
                                    int stacks) {

  std::vector<Triangle> ans;
  std::vector<Point> prev;

  for (int i = 0; i <= stacks; i++) {
    float r = radius * (stacks - i) / stacks;

    std::vector<Point> cur =
        generateCircle({0, height * i / stacks, 0}, r, slices, 0);

    if (i == 0)
      generatePolygon(cur, ans);
    else
      for (int j = 0; j < slices; j++)
        generateSquare(prev.at((j + 1) % slices), prev.at(j), cur.at(j),
                       cur.at((j + 1) % slices), ans);

    prev = cur;
  }

  return std::make_unique<Shape>(ans);
}

std::unique_ptr<Shape> generateSphere(float radius, int slices, int stacks) {
  std::vector<Triangle> ans;
  std::vector<Point> prev;

  for (int i = 0; i <= stacks; i++) {
    float h = radius * (2.0f * i / stacks - 1);
    float r = sqrt(radius * radius - h * h);

    std::vector<Point> cur = generateCircle({0, h, 0}, r, slices, 0);

    if (i != 0)
      for (int j = 0; j < slices; j++)
        generateSquare(prev.at((j + 1) % slices), prev.at(j), cur.at(j),
                       cur.at((j + 1) % slices), ans);

    prev = cur;
  }

  return std::make_unique<Shape>(ans);
}

std::unique_ptr<Shape> generateFromObj(std::string srcFile) {
  // We add one dummy vertex at the start as to shift indices by 1,
  // as vectors start at 0 but OBJ files start at 1
  auto vertices = std::vector<Point>({{0, 0, 0}});
  auto triangles = std::vector<Triangle>();

  std::ifstream file = std::ifstream(srcFile);

  std::string line = "";
  const std::regex vertex(R"(v +(-?\d+\.?\d*) (-?\d+\.?\d*) (-?\d+\.?\d*))");
  const std::regex face(
      R"(f +((\d+)(\/?\d*\/?\d*)? ){2,}((\d+)(\/?\d*\/?\d*)?))");
  const std::regex face_vertice(R"((\d+)(\/?\d*\/?\d*))");

  while (std::getline(file, line)) {
    std::smatch match;
    if (std::regex_search(line, match, vertex)) { // Is a vertex
      float x = std::stof(match[1].str());
      float y = std::stof(match[2].str());
      float z = std::stof(match[3].str());
      vertices.push_back({x, y, z});
    } else if (std::regex_search(line, match, face)) { // Is a face
      auto temp = std::vector<Point>();

      while (std::regex_search(line, match, face_vertice)) {
        int index = std::stoi(match[1]);
        temp.push_back(vertices[index]);
        line = match.suffix().str();
      }

      generatePolygon(temp, triangles);
    } // We ignore everything else
  }
  file.close();
  return std::make_unique<Shape>(triangles);
}

std::unique_ptr<Shape> generateDonut(float radius, float length, float height,
                                     int stacks, int slices) {
  std::vector<Point> *circles[2];
  circles[0] = new std::vector<Point>[stacks + 1];
  circles[1] = new std::vector<Point>[stacks + 1];

  const float stack_height = height / (float)stacks;
  const float min_y = -height / 2.0;
  const float angle_step = 2 * M_PI / slices;
  for (int i = 0; i <= stacks; i++) {
    float y = min_y + stack_height * i;
    float horizontal_gap =
        sqrt(1 - (4 * y * y) / (height * height)) * (length / 2.0);

    for (int j = 0; j < slices; j++) {
      float x = (radius - horizontal_gap) * cos(angle_step * j);
      float z = (radius - horizontal_gap) * sin(angle_step * j);

      circles[0][i].push_back({x, y, z});

      x = (radius + horizontal_gap) * cos(angle_step * j);
      z = (radius + horizontal_gap) * sin(angle_step * j);

      circles[1][i].push_back({x, y, z});
    }
  }

  std::vector<Triangle> triangles = std::vector<Triangle>();

  for (int i = 0; i < stacks; i++) {
    for (int j = 0; j < slices; j++) {
      Point p1 = circles[0][i][j];
      Point p2 = circles[0][i][(j + 1) % slices];
      Point p3 = circles[0][i + 1][j];
      Point p4 = circles[0][i + 1][(j + 1) % slices];

      triangles.push_back({p1, p2, p3});
      triangles.push_back({p3, p2, p4});

      p1 = circles[1][i][j];
      p2 = circles[1][i][(j + 1) % slices];
      p3 = circles[1][i + 1][j];
      p4 = circles[1][i + 1][(j + 1) % slices];

      triangles.push_back({p1, p3, p2});
      triangles.push_back({p2, p3, p4});
    }
  }

  delete[] circles[0];
  delete[] circles[1];

  return std::make_unique<Shape>(triangles);
}

/**
 * @brief Reads the data from a patch file.
 * 
 * Auxiliary function to #generateBezierPatches
 * 
 * The input file must have the following format:
 * 
 * A line with a single integer N. The numbers of patches in the file.
 * N lines follow. Each line contains 16 comma separated integers. The
 * indices (starting from 0) of the control points that make up the i-th patch.
 * 
 * A line with a single integer M. The number of control points. M lines follow.
 * The i-th line contains three floating point numbers: the coordinates of the i-th
 * control point.
 * 
 * The input vectors must be initialized, but their capacity can be zero.
 * 
 * @param inputFile     the path to the patch file
 * @param controlPoints the control points
 * @param patches       the patches
*/
inline void readBezierPatchFile(std::string inputFile, std::vector<Point>& controlPoints, std::vector<int[16]>& patches) {
  std::ifstream file(inputFile);

  int patchCount;
  file >> patchCount;
  patches = std::vector<int[16]>(patchCount);
  for(int i = 0; i < patchCount; i++) {
    for(int j = 0; j < 16; j++) {
      std::string str;
      file >> str;

      //Remove trailing comma if it exists
      if(str.back() == ',') {
        str.pop_back();
      }


      patches[i][j] = std::stoi(str);
    }
  }

  int controlPointCount = 0;
  file >> controlPointCount;

  for(int i = 0; i < controlPointCount; i++) {
    float coords[4];
    for(int j = 0; j < 3; j++) {
      std::string str;
      file >> str;

      if(str.back() == ',') {
        str.pop_back();
      }

      coords[j] = std::stof(str);
    }

    controlPoints.push_back(Point(coords[0], coords[1], coords[2]));
  }
  file.close();
}

/**
 * @brief Computes the triangles needed to render the bezier patches provided.
 * 
 * Auxiliary function to #generateBezierPatches
 * 
 * @param controlPoints the control points
 * @param patches       the patches
 * @param divisions     the number of divisions of a patch in one axis
 * @param triangles     the vector in which to store the triangles.
*/
inline void generateBezierTriangles(std::vector<Point>& controlPoints, std::vector<int[16]>& patches,
 int divisions, std::vector<Triangle>& triangles) {
  /*
  
  Given 16 control points p[0][0], p[0][1], ..., p[3][2], p[3][3], the Bezier patch
  is calculated as:

                           (p[0][0] p[0][1] p[0][2] p[0][3])     (v^3)
  p(u,v) = (u^3 u^2 u 1) M (p[1][0] p[1][1] p[1][2] p[1][3]) M^T (v^2)
                           (p[2][0] p[2][1] p[2][2] p[2][3])     ( v )
                           (p[3][0] p[3][1] p[3][2] p[3][3])     ( 1 )

      (-1 -3 -3 1)
  M = (3 -6  3  0)
      (-3 3  0  0)
      (1  0  0  0)
  
  Note that M = M^T

  u,v range from [0,1]. We will divide this range in `divisions`. E.g. if divisions = 4
  we will compute the patch for u,v in {0, 0.25, 0.5, 0.75, 1}

  */
  float m[16] = {
    -1.0f, 3.0f, -3.0f, 1.0f,
    3.0f, -6.0f, 3.0f, 0.0f,
    -3.0f, 3.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f
  };

  int patchCount = patches.size();
  
  Point* points = new Point[divisions * divisions];

  for(int i = 0; i < patchCount; i++) {
    //Compute the matrix that depends on the control points. The matrix is 
    //stored row-major
    Point p[16];
    for(int l = 0; l < 16; l++)
      p[l] = controlPoints[patches[i][l]];

    for(int j = 0; j < divisions; j++) {
      for(int k = 0; k < divisions; k++) {
        /*
        We need to create the vectors dependent of u and v. We need to normalize
        the indices of our loops j and k, and we do that by dividing by (divisions - 1)
        as j and k range from 0 to (divisions - 1).

        So u = j / (divisions - 1) and v = k / (divisions - 1)

        To calculate powers of u and v, note that u^n = (j / (divisions - 1))^n 
        = (j^n) / (divisions - 1)^n. The same for v

        That is why we are computing d2 and d3 and using them in the vectors
        */
        float d2 = (float)(divisions - 1)*(divisions - 1); //(divisions - 1)^2
        float d3 = (float)(divisions - 1)*d2; //(divisions - 1)^3
        float u[4] = {(float)j*j*j / d3, (float)j*j / d2, (float)j / (divisions - 1), 1};
        float v[4] = {(float)k*k*k / d3, (float)k*k / d2, (float)k / (divisions - 1), 1};
        
        /*
         Matrix multiplications
        */
        float temp1[4];
        Point temp2[16];
        matrixProd(4,1, 4, v, m, temp1);
        matrixProd(4,1, 4, temp1, p, temp2);
        Point temp3[16];
        matrixProd(4,1, 4, temp2, m, temp3);
        //Compute the result and store it in (j,k) position of the matrix
        matrixProd(1, 1, 4, temp3, u, points + j * divisions + k);
      }
    }

    /*
    
    Now with all points computed, we unite them in squares like so:
    
    p[0][0] ------ p[0][1] ----- p[0][2]
    |                 |             |
    p[1][0] ------ p[1][1] ----- p[1][2]

    */
    for(int j = 0; j < divisions - 1; j++) {
      for(int k = 0; k < divisions - 1; k++) {
        generateSquare(points[j * divisions + k], points[(j + 1) * divisions + k], points[(j + 1) * divisions + k + 1], 
          points[(j) * divisions + k + 1], triangles);
      }
    }
  }

  delete[] points;
}

std::unique_ptr<Shape> generateBezierPatches(std::string inputFile, int divisions) {
  auto controlPoints = std::vector<Point>();
  auto patches = std::vector<int[16]>();
  auto triangles = std::vector<Triangle>();

  readBezierPatchFile(inputFile, controlPoints, patches);
  generateBezierTriangles(controlPoints, patches, divisions, triangles);
  

  return std::make_unique<Shape>(triangles);
}