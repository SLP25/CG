#pragma once
#include "model.hpp"
#include "parser.hpp"
#include "transformation.hpp"

class Group {
public:
  Group();
  Group(const Group& group);
  Group(Group&& group);
  Group(XMLParser parser);

  Group& operator=(const Group& group);
  Group& operator=(Group&& other);

  void draw();

private:
  inline void assertValidXML(XMLParser parser);
  inline void constructSubGroups(XMLParser parser);
  inline void constructModels(XMLParser parser);
  inline void constructTransformations(XMLParser parser);

private:
  std::vector<Group> subgroups;
  std::vector<Model> models;
  std::vector<std::unique_ptr<Transformation>> transformations;
};
