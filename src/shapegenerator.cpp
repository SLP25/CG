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

std::vector<Point2D> generateTextureCoordinates(std::vector<Triangle>& triangles, std::map<Point, Point2D>& textures) {
  std::vector<Point2D> ans = std::vector<Point2D>();
  
  for(Triangle t : triangles) {
    ans.push_back(textures[std::get<0>(t)]);
    ans.push_back(textures[std::get<1>(t)]);
    ans.push_back(textures[std::get<2>(t)]);
  }

  return ans;
}


// displacement is a fraction of the angle of slicing. 0.5 displacement means
// the circle is offset by PI/slices rad
std::vector<Point> generateCircle(Point center, float radius, int slices,
                                  float displacement) {
  std::vector<Point> ans;

  for (int i = 0; i < slices; i++) {
    float angle = 2 * M_PI * (i + displacement) / slices;
    Vector v = {radius * cos(angle), 0, radius * sin(angle)};
    ans.push_back(center + v);
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

  1. Compute all the relevant points in the plane and their respective
  texture mapping
  2. Create a mapping between these points and their indices in the
  vector that stores them. This will be useful when computing the faces
  3. Compute the vertices belonging to each face. Use the reverse mapping
  to store the indices of all vertices of a face. They must be generated
  in counter-clockwise fashion

  Texture mapping is trivial. Point (x,y) will have texture coordinate
  ( (x + (length / 2))/ length, (y / (length / 2)) / length)

  Normals are simply equal to (0,1,0)

  */
  std::vector<Triangle> triangles;
  std::vector<Vector> normals;
  std::vector<Point2D> textureMapping;

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

  for(Triangle t : triangles) {
    Point p[3] = {std::get<0>(t), std::get<1>(t), std::get<2>(t)};
    for(int i = 0; i < 3; i++) {
      float x = std::get<0>(p[i]);
      float z = std::get<2>(p[i]);

      textureMapping.push_back({(x + mid) / length, (z + mid) / length});
      normals.push_back({0.0f, 1.0f, 0.0f});
    }
  }

  return std::make_unique<Shape>(triangles, normals, textureMapping);
}

std::unique_ptr<Shape> generateCube(float length, int divisions) {
  std::vector<Triangle> triangles;
  std::vector<Vector> normals;
  std::vector<Point2D> textureMapping;

  float mid = length / 2.0;
  float step = length / divisions;
  int count = 0;

  for (int i = 0; i < divisions; i++) {
    for (int j = 0; j < divisions; j++) {
      generateSquare({-mid, -mid + i * step, -mid + j * step},
                     {-mid, -mid + i * step, -mid + (j + 1) * step},
                     {-mid, -mid + (i + 1) * step, -mid + (j + 1) * step},
                     {-mid, -mid + (i + 1) * step, -mid + j * step}, triangles);
    }
  }
  float p_x, p_y, p_z;
  p_x = -mid;
  p_y = -mid;
  p_z = -mid;

  //Face 5
  for(; count < (int)triangles.size(); count++) {
    Point p[3] = {std::get<0>(triangles[count]), std::get<1>(triangles[count]), std::get<2>(triangles[count])};
    for(int i = 0; i < 3; i++) {
      float y = std::get<1>(p[i]);
      float z = std::get<2>(p[i]);

      float u = (z - p_z) / length;
      float v = (y - p_y) / length;

      u = (u / 4.0f) + 0.75f;
      v = (v / 3.0f) + (1.0f / 3.0f);

      textureMapping.push_back({u, v});
      normals.push_back({-1.0f, 0.0f, 0.0f});
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

  p_x = mid;
  p_y = -mid;
  p_z = -mid;

  //Face 3
  for(; count < (int)triangles.size(); count++) {
    Point p[3] = {std::get<0>(triangles[count]), std::get<1>(triangles[count]), std::get<2>(triangles[count])};
    for(int i = 0; i < 3; i++) {
      float z = std::get<2>(p[i]);
      float y = std::get<1>(p[i]);

      float u = (z - p_z) / length;
      float v = (y - p_y) / length;

      u = 0.5f - (u / 4.0f) ;
      v = (v / 3.0f) + (1.0f / 3.0f);

      textureMapping.push_back({u, v});
      normals.push_back({1.0f, 0.0f, 0.0f});
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

  p_x = -mid;
  p_y = -mid;
  p_z = -mid;

  //Face 1
  for(; count < (int)triangles.size(); count++) {
    Point p[3] = {std::get<0>(triangles[count]), std::get<1>(triangles[count]), std::get<2>(triangles[count])};
    for(int i = 0; i < 3; i++) {
      float x = std::get<0>(p[i]);
      float z = std::get<2>(p[i]);

      float v = (x - p_x) / length;
      float u = 1.0f - (z - p_z) / length;

      u = (u / 4.0f) + 0.25f;
      v = (v / 3.0f);

      textureMapping.push_back({u, v});
      normals.push_back({0.0f, -1.0f, 0.0f});
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

  p_x = mid;
  p_y = mid;
  p_z = mid;

  //Face 6
  for(; count < (int)triangles.size(); count++) {
    Point p[3] = {std::get<0>(triangles[count]), std::get<1>(triangles[count]), std::get<2>(triangles[count])};
    for(int i = 0; i < 3; i++) {
      float x = std::get<0>(p[i]);
      float z = std::get<2>(p[i]);

      float v = (p_x - x) / length;
      float u = (p_z - z) / length;

      u = (u / 4.0f) + 0.25f;
      v = (v / 3.0f) + (2.0f / 3.0f);

      textureMapping.push_back({u, v});
      normals.push_back({1.0f, 0.0f, 0.0f});
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

  p_x = -mid;
  p_y = -mid;
  p_z = -mid;

  //Face 2
  for(; count < (int)triangles.size(); count++) {
    Point p[3] = {std::get<0>(triangles[count]), std::get<1>(triangles[count]), std::get<2>(triangles[count])};
    for(int i = 0; i < 3; i++) {
      float y = std::get<1>(p[i]);
      float x = std::get<0>(p[i]);

      float u = (p_x - x) / length;
      float v = (y - p_y) / length;

      u = (u / 4.0f) - 0.75f;
      v = (v / 3) + (1.0f / 3.0f);

      textureMapping.push_back({u, v});
      normals.push_back({0.0f, 0.0f, -1.0f});
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
  
  p_x = mid;
  p_y = -mid;
  p_z = mid;

  //Face 4
  for(; count < (int)triangles.size(); count++) {
    Point p[3] = {std::get<0>(triangles[count]), std::get<1>(triangles[count]), std::get<2>(triangles[count])};
    for(int i = 0; i < 3; i++) {
      float y = std::get<1>(p[i]);
      float x = std::get<0>(p[i]);

      float u = (x - p_x) / length;
      float v = (y - p_y) / length;

      u = (u / 4.0f) + 0.75f;
      v = (v / 3.0f) + (1.0f / 3.0f);

      textureMapping.push_back({u, v});
      normals.push_back({0.0f, 0.0f, 1.0f});
    }
  }


  return std::make_unique<Shape>(triangles, normals, textureMapping);
}

std::unique_ptr<Shape> generateCylinder(float radius, float height,
                                        int slices) {
  std::vector<Point> top = generateCircle({0, height, 0}, radius, slices, 0);
  std::vector<Point> bottom = generateCircle({0, 0, 0}, radius, slices, 0.5);
  std::vector<Triangle> ans;
  std::vector<Vector> normals;
  std::vector<Point2D> textures;

  for (int i = 0; i < slices; i++) {
    int i2 = (i + 1) % slices;
    ans.push_back({top[i], top[i2], bottom[i]});
    ans.push_back({bottom[i2], bottom[i], top[i2]});

    normals.push_back(normalize({std::get<0>(top[i]), 0.0f, std::get<2>(top[i])}));
    normals.push_back(normalize({std::get<0>(top[i2]), 0.0f, std::get<2>(top[i2])}));
    normals.push_back(normalize({std::get<0>(bottom[i]), 0.0f, std::get<2>(bottom[i])}));
    normals.push_back(normalize({std::get<0>(bottom[i2]), 0.0f, std::get<2>(bottom[i2])}));
    normals.push_back(normalize({std::get<0>(bottom[i]), 0.0f, std::get<2>(bottom[i])}));
    normals.push_back(normalize({std::get<0>(top[i2]), 0.0f, std::get<2>(top[i2])}));

    i2 = i == slices - 1 ? slices : i2;
    textures.push_back({1.0f - (float)i / slices, 1.0f});
    textures.push_back({1.0f - (float)i2 / slices, 1.0f});
    textures.push_back({1.0f - (float)i / slices, 0.375f});
    textures.push_back({1.0f - (float)i2 / slices, 0.375f});
    textures.push_back({1.0f - (float)i / slices, 0.375f});
    textures.push_back({1.0f - (float)i2 / slices, 1.0f});

    
  }
  int count = ans.size();
  reverse(top.begin(), top.end());
  generatePolygon(top, ans);

  for(; count < (int)ans.size(); count++) {
    Point p[3] = {std::get<0>(ans[count]), std::get<1>(ans[count]), std::get<2>(ans[count])};
    for(int i = 0; i < 3; i++) {
      float x = std::get<0>(p[i]);
      float z = std::get<2>(p[i]);

      float u = x / (2 * radius) + 0.5f;
      float v = z / (2 * radius) + 0.5f;
      v = 1 - v;
      u *= 0.375;
      v *= 0.375;
      u += 0.25;
      textures.push_back({u,v});
      normals.push_back({0.0f, 1.0f, 0.0f});
    }     
  }

  generatePolygon(bottom, ans);

  for(; count < (int)ans.size(); count++) {
    Point p[3] = {std::get<0>(ans[count]), std::get<1>(ans[count]), std::get<2>(ans[count])};
    for(int i = 0; i < 3; i++) {
      float x = std::get<0>(p[i]);
      float z = std::get<2>(p[i]);

      float u = x / (2 * radius) + 0.5f;
      float v = z / (2 * radius) + 0.5f;
      v = 1 - v;
      u *= 0.375;
      v *= 0.375;
      u += 0.625;
      textures.push_back({u,v});
      normals.push_back({0.0f, -1.0f, 0.0f});
    }     
  }

  return std::make_unique<Shape>(ans, normals, textures);
}

std::unique_ptr<Shape> generateCone(float radius, float height, int slices,
                                    int stacks) {

  std::vector<Triangle> ans;
  std::vector<Vector> normals;
  std::vector<Point> prev;
  std::vector<Point2D> textures;
  
  int count = 0;

  float angle = atan(radius / height);

  for (int i = 0; i <= stacks; i++) {
    float r = radius * (stacks - i) / stacks;

    std::vector<Point> cur =
        generateCircle({0, height * i / stacks, 0}, r, slices, 0);

    if(i == 0) {
      generatePolygon(cur, ans);

      for(; count < (int)ans.size(); count++) {
        Point p[3] = {std::get<0>(ans[count]), std::get<1>(ans[count]), std::get<2>(ans[count])};
        for(int i = 0; i < 3; i++) {
          float x = std::get<0>(p[i]);
          float z = std::get<2>(p[i]);

          float u = x / (2 * radius) + 0.5f;
          float v = z / (2 * radius) + 0.5f;
          textures.push_back({u,v});
          normals.push_back({0.0f, -1.0f, 0.0f});
        }     
      }
    } else {
      for (int j = 0; j < slices; j++) {
        Point p1 = prev.at((j + 1) % slices);
        Point p2 = prev.at(j);
        Point p3 = cur.at(j);
        Point p4 = cur.at((j + 1) % slices);

        ans.push_back({p1, p2, p3});
        ans.push_back({p1, p3, p4});

        textures.push_back({(float)(j + 1) / slices, (float)(i - 1) / stacks});
        textures.push_back({(float)j / slices, (float)(i - 1) / stacks});
        textures.push_back({(float)j / slices, (float)i / stacks});
        textures.push_back({(float)(j + 1) / slices, (float)(i - 1) / stacks});
        textures.push_back({(float)j / slices, (float)i / stacks});
        textures.push_back({(float)(j + 1) / slices, (float)i / stacks});

        if(i == stacks) {
          for(int k = 0; k < 6; k++)
            normals.push_back({0.0f, 1.0f, 0.0f});

        } else {
          Vector vertical = {0.0f, sin(angle), 0.0f};
          normals.push_back((cos(angle)) * normalize(projectToPlane({0.0f, 1.0f, 0.0f}, p1)) + vertical);
          normals.push_back((cos(angle)) * normalize(projectToPlane({0.0f, 1.0f, 0.0f}, p2)) + vertical);
          normals.push_back((cos(angle)) * normalize(projectToPlane({0.0f, 1.0f, 0.0f}, p3)) + vertical);
          normals.push_back((cos(angle)) * normalize(projectToPlane({0.0f, 1.0f, 0.0f}, p1)) + vertical);
          normals.push_back((cos(angle)) * normalize(projectToPlane({0.0f, 1.0f, 0.0f}, p3)) + vertical);
          normals.push_back((cos(angle)) * normalize(projectToPlane({0.0f, 1.0f, 0.0f}, p4)) + vertical);
        }
      }
    }

    prev = cur;
  }

  return std::make_unique<Shape>(ans, normals, textures);
}

std::unique_ptr<Shape> generateSphere(float radius, int slices, int stacks) {
  std::vector<Triangle> ans;
  std::vector<Vector> normals;
  std::vector<Point2D> textureMapping;
  std::vector<Point> prev;

  std::map<Point, Point2D> textureMap;

  for (int i = 0; i <= stacks; i++) {
    float h = radius * (2.0f * i / stacks - 1);
    float r = sqrt(radius * radius - h * h);

    std::vector<Point> cur = generateCircle({0, h, 0}, r, slices, 0);
    cur.push_back(cur[0]);

    for(Point p : cur) {
      // Source: https://en.wikipedia.org/wiki/UV_mapping
      // Computing texture coordinates based on lattitude and longitude
      float u = 0.5f - (atan2(std::get<2>(p) / radius, std::get<0>(p) / radius)) / (2 * M_PI);
      float v = 0.5f + asin(std::get<1>(p) / radius) / M_PI;

      textureMap[p] = {u, v};
    }

    if (i != 0)
      for (int j = 0; j < slices; j++) {
        Point p1 = prev.at(j + 1);
        Point p2 = prev.at(j);
        Point p3 = cur.at(j);
        Point p4 = cur.at(j + 1);

        ans.push_back({p1, p2, p3});
        ans.push_back({p1, p3, p4});

        float u = 0.5f - (atan2(std::get<2>(p2) / radius, std::get<0>(p2) / radius)) / (2 * M_PI);
        float v = 0.5f + asin(std::get<1>(p2) / radius) / M_PI;

        textureMapping.push_back({u - 1.0f / slices,v});
        textureMapping.push_back({u,v});
        textureMapping.push_back({u,v + 1.0f / stacks});
        textureMapping.push_back({u - 1.0f / slices,v});
        textureMapping.push_back({u,v + 1.0f / stacks});
        textureMapping.push_back({u - 1.0f / slices,v + 1.0f / stacks});

        /*u = 0.5f - (atan2(std::get<2>(p2) / radius, std::get<0>(p2) / radius)) / (2 * M_PI);
        v = 0.5f + asin(std::get<1>(p2) / radius) / M_PI;
        textureMapping.push_back({u,v});

        u = 0.5f - (atan2(std::get<2>(p3) / radius, std::get<0>(p3) / radius)) / (2 * M_PI);
        v = 0.5f + asin(std::get<1>(p3) / radius) / M_PI;
        textureMapping.push_back({u,v});

        u = j == slices - 1 ? 1.0f : 0.5f - (atan2(std::get<2>(p1) / radius, std::get<0>(p1) / radius)) / (2 * M_PI);
        v = 0.5f + asin(std::get<1>(p1) / radius) / M_PI;
        textureMapping.push_back({u,v});

        u = 0.5f - (atan2(std::get<2>(p3) / radius, std::get<0>(p3) / radius)) / (2 * M_PI);
        v = 0.5f + asin(std::get<1>(p3) / radius) / M_PI;
        textureMapping.push_back({u,v});

        u = j == slices - 1 ? 1.0f : 0.5f - (atan2(std::get<2>(p4) / radius, std::get<0>(p4) / radius)) / (2 * M_PI);
        v = 0.5f + asin(std::get<1>(p4) / radius) / M_PI;
        textureMapping.push_back({u,v});*/
      }

    prev = cur;
  }

  for(Triangle t : ans) {
    Point p[3] = {std::get<0>(t), std::get<1>(t), std::get<2>(t)};

    for(int j = 0; j < 3; j++) {
      textureMapping.push_back(textureMap[p[j]]);
      normals.push_back(normalize(p[j]));
    }
  }

  return std::make_unique<Shape>(ans, normals, textureMapping);
}

std::unique_ptr<Shape> generateFromObj(std::string srcFile) {
  // We add one dummy vertex at the start as to shift indices by 1,
  // as vectors start at 0 but OBJ files start at 1
  auto vertices = std::vector<Point>({{0, 0, 0}});
  auto textures = std::vector<Point2D>({{0,0}});
  auto normals = std::vector<Vector>({{0,0,0}});

  auto triangles = std::vector<Triangle>();
  auto textureMapping = std::vector<Point2D>();
  auto normalMapping = std::vector<Vector>();

  std::ifstream file = std::ifstream(srcFile);

  std::string line = "";
  const std::regex vertex(R"(v +(-?\d+\.?\ std::vector<Vector>()d*) (-?\d+\.?\d*) (-?\d+\.?\d*))");
  const std::regex face(
      R"(f +((\d+)(\/?\d*\/?\d*)? ){2,}((\d+)(\/?\d*\/?\d*)?))");
  const std::regex textureRegex(R"(vt +(\d+\.?\d*) +(\d+\.?\d*))");
  const std::regex normalRegex(R"(vn +(\d+\.?\d*) +(\d+\.?\d*))");
  const std::regex face_vertice(R"((\d+)\/?(\d*)\/?(\d*))");
  int count = 0;
  while (std::getline(file, line)) {
    std::smatch match;
    if (std::regex_search(line, match, vertex)) { // Is a vertex
      float x = std::stof(match[1].str());
      float y = std::stof(match[2].str());
      float z = std::stof(match[3].str());
      vertices.push_back({x, y, z});
    } else if (std::regex_search(line, match, normalRegex)) { // Is a normal
      float x = std::stof(match[1].str());
      float y = std::stof(match[2].str());
      float z = std::stof(match[3].str());
      normals.push_back({x, y, z});
    } else if(std::regex_search(line, match, textureRegex)) { // Is a texture
      float u = std::stof(match[1].str());
      float v = 1.0f - std::stof(match[2].str());
      textures.push_back({u,v}); 
    } else if (std::regex_search(line, match, face)) { // Is a face
      auto temp = std::vector<Point>();
      auto textureMap = std::map<Point, Point2D>();
      auto normalMap = std::map<Point, Vector>();

      while (std::regex_search(line, match, face_vertice)) {
        int index = std::stoi(match[1]);
        temp.push_back(vertices[index]);

        if(match[1] != "") {
          int j = std::stoi(match[2]);
          normalMap[vertices[index]] = normals[j];
        }

        if(match[2] != "") {
          int j = std::stoi(match[2]);
          textureMap[vertices[index]] = textures[j];
        }

        line = match.suffix().str();
      }

      triangles.push_back({temp[0], temp[1], temp[2]});
      for(; count < (int)triangles.size(); count++) {
        Point p[3] = {std::get<0>(triangles[count]), std::get<1>(triangles[count]), std::get<2>(triangles[count])};

        for(int k = 0; k < 3; k++) {
          if(textureMap.find(p[k]) == textureMap.end()) {
            textureMapping.push_back({0,0});
            normalMapping.push_back({0,0,0});
          } else {
            textureMapping.push_back(textureMap[p[k]]);
            normalMapping.push_back(normalMap[p[k]]);
          }
        }
      }
    } // We ignore everything else
  }

  //TODO: normals

  file.close();
  return std::make_unique<Shape>(triangles, normalMapping, textureMapping);
}

std::unique_ptr<Shape> generateDonut(float radius, float length, float height,
                                     int stacks, int slices) {
  std::vector<Point> *circles[2];
  std::vector<Vector> *normalsTemp[2];
  circles[0] = new std::vector<Point>[stacks + 1];
  circles[1] = new std::vector<Point>[stacks + 1];
  normalsTemp[0] = new std::vector<Vector>[stacks + 1];
  normalsTemp[1] = new std::vector<Vector>[stacks + 1];


  const float stack_height = height / (float)stacks;
  const float min_y = -height / 2.0;
  const float angle_step = 2 * M_PI / slices;
  const float a = length / 2.0;
  const float b = height / 2.0;
  for (int i = 0; i <= stacks; i++) {
    float y = min_y + stack_height * i;
    float horizontal_gap =
        sqrt(1 - (4 * y * y) / (height * height)) * (length / 2.0);

    for (int j = 0; j < slices; j++) {
      float x = (radius - horizontal_gap) * cos(angle_step * j);
      float z = (radius - horizontal_gap) * sin(angle_step * j);

      float w = x*x + z*z - radius;
      float gamma = b*b * w*w / (a*sqrt((1 - b*b*x*x) / a));

      circles[0][i].push_back({x, y, z});
      normalsTemp[0][i].push_back(normalize({sin(angle_step * j), gamma, cos(angle_step * j)}));

      x = (radius + horizontal_gap) * cos(angle_step * j);
      z = (radius + horizontal_gap) * sin(angle_step * j);
      w = x*x + z*z - radius;

      gamma = - b*b * w*w / (a*sqrt((1 - b*b*x*x) / a));
      circles[1][i].push_back({x, y, z});
      normalsTemp[1][i].push_back(normalize({sin(angle_step * j), gamma, cos(angle_step * j)}));
    }
  }

  std::vector<Triangle> triangles = std::vector<Triangle>();
  std::vector<Vector> normals;
  std::vector<Point2D> textures;

  for (int i = 0; i < stacks; i++) {
    for (int j = 0; j < slices; j++) {
      Point p1 = circles[0][i][j];
      Point p2 = circles[0][i][(j + 1) % slices];
      Point p3 = circles[0][i + 1][j];
      Point p4 = circles[0][i + 1][(j + 1) % slices];

      triangles.push_back({p1, p2, p3});
      triangles.push_back({p3, p2, p4});

      normals.push_back(normalsTemp[0][i][j]);
      normals.push_back(normalsTemp[0][i][(j + 1) % slices]);
      normals.push_back(normalsTemp[0][i + 1][j]);
      normals.push_back(normalsTemp[0][i + 1][j]);
      normals.push_back(normalsTemp[0][i + 1][(j + 1) % slices]);
      normals.push_back(normalsTemp[0][i + 1][(j + 1) % slices]);

      p1 = circles[1][i][j];
      p2 = circles[1][i][(j + 1) % slices];
      p3 = circles[1][i + 1][j];
      p4 = circles[1][i + 1][(j + 1) % slices];

      triangles.push_back({p1, p3, p2});
      triangles.push_back({p2, p3, p4});

      normals.push_back(normalsTemp[1][i][j]);
      normals.push_back(normalsTemp[1][i + 1][j]);
      normals.push_back(normalsTemp[1][i][(j + 1) % slices]);
      normals.push_back(normalsTemp[1][i + 1][(j + 1) % slices]);
      normals.push_back(normalsTemp[1][i + 1][j]);
      normals.push_back(normalsTemp[1][i + 1][(j + 1) % slices]);
    }
  }

  delete[] circles[0];
  delete[] circles[1];
  delete[] normalsTemp[0];
  delete[] normalsTemp[1];

  for(int i = 0; i < (int)normals.size(); i++)
    textures.push_back({0,0});

  return std::make_unique<Shape>(triangles, normals, textures);
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
 * @param triangles     the vector in which to store the triangles
 * @param normals       the vector in which to store the vertex normals
 * @param textures      the vector in which to store the texture coordinates
*/
inline void generateBezierTriangles(std::vector<Point>& controlPoints, std::vector<int[16]>& patches,
 int divisions, std::vector<Triangle>& triangles, std::vector<Vector>& normals, std::vector<Point2D>& textures) {
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
  Vector* normalArr = new Vector[divisions * divisions];

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
        float du[4] = {(float)3*j*j / d3, (float)2*j / d2, 1.0f, 0.0f};
        float v[4] = {(float)k*k*k / d3, (float)k*k / d2, (float)k / (divisions - 1), 1};
        float dv[4] = {(float)3*k*k / d3, (float)2*k / d2, 1.0f, 0.0f};
        
        /*
         Matrix multiplications: Coordinates
        */
        float temp1[4];
        Point temp2[16];
        matrixProd(4,1, 4, v, m, temp1);
        matrixProd(4,1, 4, temp1, p, temp2);
        Point temp3[16];
        matrixProd(4,1, 4, temp2, m, temp3);
        //Compute the result and store it in (j,k) position of the matrix
        matrixProd(1, 1, 4, temp3, u, points + j * divisions + k);

         /*
         Matrix multiplications: Normals
        */
        //du
        Vector du_vector;
        matrixProd(4,1, 4, v, m, temp1);
        matrixProd(4,1, 4, temp1, p, temp2);
        matrixProd(4,1, 4, temp2, m, temp3);
        //Compute the result and store it in (j,k) position of the matrix
        matrixProd(1, 1, 4, temp3, du, &du_vector);
        //du
        Vector dv_vector;
        matrixProd(4,1, 4, dv, m, temp1);
        matrixProd(4,1, 4, temp1, p, temp2);
        matrixProd(4,1, 4, temp2, m, temp3);
        //Compute the result and store it in (j,k) position of the matrix
        matrixProd(1, 1, 4, temp3, u, &dv_vector);

        Vector temp = du_vector ^ dv_vector;
        if(length(temp) == 0)
          temp = {0.0f, 1.0f, 0.0f};
        else
          temp = normalize(temp);
        normalArr[j * divisions + k] = temp;
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
        Point p1 = points[j * divisions + k];
        Point p2 = points[(j + 1) * divisions + k];
        Point p3 = points[(j + 1) * divisions + k + 1];
        Point p4 = points[(j) * divisions + k + 1];

        triangles.push_back({p1, p2, p3});
        triangles.push_back({p1, p3, p4});

        textures.push_back({(float)j / (divisions - 1), (float)k / (divisions - 1)});
        textures.push_back({(float)(j + 1) / (divisions - 1), (float)k / (divisions - 1)});
        textures.push_back({(float)(j + 1) / (divisions - 1), (float)(k + 1) / (divisions - 1)});
        textures.push_back({(float)j / (divisions - 1), (float)k / (divisions - 1)});
        textures.push_back({(float)(j + 1) / (divisions - 1), (float)(k + 1) / (divisions - 1)});
        textures.push_back({(float)j / (divisions - 1), (float)(k + 1) / (divisions - 1)});

        normals.push_back(normalArr[j * divisions + k]);
        normals.push_back(normalArr[(j + 1) * divisions + k]);
        normals.push_back(normalArr[(j + 1) * divisions + k + 1]);
        normals.push_back(normalArr[j * divisions + k]);
        normals.push_back(normalArr[(j + 1) * divisions + k + 1]);
        normals.push_back(normalArr[(j) * divisions + k + 1]);
      }
    }
  }

  delete[] points;
  delete[] normalArr;
}

std::unique_ptr<Shape> generateBezierPatches(std::string inputFile, int divisions) {
  auto controlPoints = std::vector<Point>();
  auto patches = std::vector<int[16]>();
  auto triangles = std::vector<Triangle>();
  auto normals = std::vector<Vector>();
  auto textures = std::vector<Point2D>();

  readBezierPatchFile(inputFile, controlPoints, patches);
  generateBezierTriangles(controlPoints, patches, divisions, triangles, normals, textures);
  

  return std::make_unique<Shape>(triangles, normals, textures);
}