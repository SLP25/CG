#ifdef __APPLE__
#include <GLUT/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "../include/group.hpp"
#include "../include/model.hpp"

Group::Group(XMLParser parser) {
  this->subgroups = std::vector<std::unique_ptr<Group>>();
  this->models = std::vector<std::unique_ptr<Model>>();
  this->transformations = std::vector<int>();

  assertValidXML(parser);

  constructSubGroups(parser);
  constructModels(parser);
  constructTransformations(parser);
}

void Group::draw() {
  glPushMatrix();
  for (auto &m : this->models) {
    m->draw();
  }

  for (auto &g : this->subgroups) {
    g->draw();
  }
  glPopMatrix();
}

void Group::assertValidXML(XMLParser parser) {
  parser.validate_node({"transform", "models", "group"});
  parser.validate_attrs({});
  parser.validate_max_nodes(1, {"transform", "models"});
}

void Group::constructSubGroups(XMLParser parser) {
  std::vector<XMLParser> groups = parser.get_nodes("group");
  for (XMLParser group : groups) {
    this->subgroups.push_back(std::make_unique<Group>(group));
  }
}

void Group::constructModels(XMLParser parser) {
  XMLParser modelsTag = parser.get_node("models");
  std::vector<XMLParser> modelTags = modelsTag.get_nodes("model");
  for (XMLParser model : modelTags) {
    this->models.push_back(std::make_unique<Model>(model));
  }
}

void Group::constructTransformations(XMLParser parser) {
  XMLParser transformationTags = parser.get_node("transform");
  std::vector<XMLParser> transformTags = transformationTags.get_nodes();
  for (XMLParser transformTag : transformTags) {
    // this->transformations.push_back(Transformation.parse(transformTag));
  }
}
