#ifdef __APPLE__
#include <GLUT/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <iostream>

#include "group.hpp"
#include "model.hpp"
#include "transformation.hpp"

Group::Group() { }

Group::Group(const Group& group) :
  subgroups(group.subgroups),
  models(group.models)
{
  for (auto& t : group.transformations) {
    Transformation* copy = t->clone();
    std::unique_ptr<Transformation> ptr(copy);
    this->transformations.push_back(std::move(ptr));
  }
}

Group::Group(Group&& group) :
  subgroups(std::move(group.subgroups)),
  models(std::move(group.models)),
  transformations(std::move(group.transformations))
{}

Group::Group(XMLParser parser) {
  assertValidXML(parser);

  constructSubGroups(parser);
  constructModels(parser);
  constructTransformations(parser);
}

Group& Group::operator=(const Group& group) {
  this->subgroups = group.subgroups;
  this->models = group.models;

  for (auto& t : group.transformations) {
    Transformation* copy = t->clone();
    std::unique_ptr<Transformation> ptr(copy);
    this->transformations.push_back(std::move(ptr));
  }

  return *this;
}

Group& Group::operator=(Group&& group) {
  this->subgroups = std::move(group.subgroups);
  this->models = std::move(group.models);
  this->transformations = std::move(group.transformations);
  return *this;
}

void Group::draw() {
  glPushMatrix();

  for (auto &t : this->transformations) {
    t->apply();
  }

  for (auto &m : this->models) {
    m.draw();
  }

  for (auto &g : this->subgroups) {
    g.draw();
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
    this->subgroups.push_back(group);
  }
}

void Group::constructModels(XMLParser parser) {
  std::vector<XMLParser> modelsTags = parser.get_nodes("models");
  for (XMLParser modelsTag : modelsTags) {
    std::vector<XMLParser> modelTags = modelsTag.get_nodes("model");
    for (XMLParser model : modelTags) {
      this->models.push_back(model);
    }
  }
}

void Group::constructTransformations(XMLParser parser) {
  std::vector<XMLParser> transformationTags = parser.get_nodes("transform");
  for (XMLParser transformationTag : transformationTags) {
    std::vector<XMLParser> transformTags = transformationTag.get_nodes();
    for (XMLParser transformTag : transformTags) {
      this->transformations.push_back(std::move(Transformation::parse(transformTag)));
    }
  }
}
