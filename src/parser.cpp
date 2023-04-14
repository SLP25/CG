/**
 * @file parser.cpp
 *
 * @brief File implementing the XMLParser class.
 */

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <tuple>

#include "parser.hpp"
#include "exceptions/invalid_xml_file.hpp"
#include "parser.hpp"

XMLParser::XMLParser(std::shared_ptr<std::string> content,
                     std::shared_ptr<rapidxml::xml_document<>> doc,
                     rapidxml::xml_node<> *node) {
  this->content = content;
  this->doc = doc;
  this->node = node;
}

XMLParser::XMLParser(std::string file_path) {

  std::ifstream file(file_path);

  if (!file)
    throw InvalidXMLStructure(
        "engine:XMLParser : The XML file provided does not exits.");

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  content = std::make_shared<std::string>(buffer.str());
  doc = std::make_shared<rapidxml::xml_document<>>();

  doc->parse<0>(&(*content)[0]);
  node = &(*doc);
}

std::string XMLParser::name() { return node->name(); }

void XMLParser::validate_node(std::initializer_list<std::string> names) {

  std::set<std::string> aux(names);
  std::stringstream exception_message;

  for (rapidxml::xml_node<> *n = node->first_node(); n; n = n->next_sibling())

    if (aux.find(std::string(n->name())) == aux.end()) {
      std::cout << *names.begin() << *(names.begin() + 1) << std::endl;
      exception_message << "XMLParser: The node '" << std::string(n->name())
                        << "' is not valid in node '" << name() << "'.";
      throw InvalidXMLStructure(exception_message.str());
    }
}

void XMLParser::validate_max_nodes(int max,
                                   std::initializer_list<std::string> names) {

  std::map<std::string, int> aux;
  std::stringstream exception_message;

  for (rapidxml::xml_node<> *n = node->first_node(); n; n = n->next_sibling()) {
    std::string name(n->name());
    aux[name]++;
  }

  for (std::string name : names) {
    if (aux[name] > max) {

      exception_message << "XMLParser: Expected at max " << std::to_string(max)
                        << " subnodes '" << name
                        << "', but got " << std::to_string(aux[name])
                        << " in node '" << this->name() << "'.";
      throw InvalidXMLStructure(exception_message.str());
    }
  }
}

std::vector<XMLParser> XMLParser::get_nodes() {
  std::vector<XMLParser> ans;

  for (rapidxml::xml_node<> *n = node->first_node(); n; n = n->next_sibling())
    ans.push_back(XMLParser(content, doc, n));

  return ans;
}

std::vector<XMLParser> XMLParser::get_nodes(std::string name) {
  std::vector<XMLParser> ans;

  for (rapidxml::xml_node<> *n = node->first_node(); n; n = n->next_sibling())
    if (std::string(n->name()) == name)
      ans.push_back(XMLParser(content, doc, n));

  return ans;
}

XMLParser XMLParser::get_node(std::string name) {

  std::stringstream exception_message;

  for (rapidxml::xml_node<> *n = node->first_node(); n; n = n->next_sibling())
    if (std::string(n->name()) == name)
      return XMLParser(content, doc, n);

  exception_message << "XMLParser: The child node '" << name << "' of node '"
                    << this->name() << "' does not exist.";
  throw InvalidXMLStructure(exception_message.str());
}

void XMLParser::validate_attrs(std::initializer_list<std::string> attrs) {

  std::set<std::string> aux(attrs);
  std::stringstream exception_message;

  for (rapidxml::xml_attribute<> *attr = node->first_attribute(); attr;
       attr = attr->next_attribute())

    if (aux.find(std::string(attr->name())) == aux.end()) {

      exception_message << "XMLParser: The attribute '" << attr->name()
                        << "' is not valid for the node '" << name() << "'.";
      throw InvalidXMLStructure(exception_message.str());
    }
}
