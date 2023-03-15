#pragma once

#include "parser.hpp"

class Group {
public:
  Group(XMLParser parser);
  void draw();

private:
  inline void assertValidXML(XMLParser parser);
  inline void constructSubGroups(XMLParser parser);
  inline void constructModels(XMLParser parser);
  inline void constructTransformations(XMLParser parser);

private:
  std::vector<std::unique_ptr<Group>> subgroups;
  std::vector<std::unique_ptr<Model>> models;
  std::vector<int> transformations;
};
