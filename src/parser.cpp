/**
 * @file parser.cpp
 *
 * @brief File implementing the XMLParser class.
 */

#include <fstream>
#include <tuple>
#include <cstring>
#include <sstream>
#include <iostream>
#include <memory>
#include <set>

#include "utils.hpp"
#include "parser.hpp"
#include "exceptions/invalid_xml_file.hpp"
#include "../include/parser.hpp"


using namespace std;
using namespace rapidxml;

XMLParser::XMLParser(shared_ptr<string> content, shared_ptr<xml_document<>> doc, xml_node<> *node) {
    this->content = content;
    this->doc = doc;
    this->node = node;
}

XMLParser::XMLParser(string file_path) {
    ifstream file(file_path);

    if (!file)
        throw InvalidXMLStructure("engine:XMLParser : The XML file provided does not exits.");

    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    content = make_shared<string>(buffer.str());
    doc = make_shared<xml_document<>>();
    
    doc->parse<0>(&(*content)[0]);
    node = &(*doc);
}

string XMLParser::name() {
    return node->name();
}

void XMLParser::validate_node(initializer_list<string> names) {

    set<string> aux(names);
    std::stringstream exception_message;

    for (xml_node<> *n = node->first_node(); n; n = n->next_sibling())

        if (aux.find(string(n->name())) == aux.end()) {

            exception_message << "XMLParser: The node '" << string(n->name()) << "' is not valid in node '" << name() << "'.";
            throw InvalidXMLStructure(exception_message.str());

        }
}

void XMLParser::validate_max_nodes(int max, initializer_list<string> names) {

    std::map<string,int> aux;
    std::stringstream exception_message;

    for (string name : names) aux[name] = 0;

    for (xml_node<> *n = node->first_node(); n; n = n->next_sibling()) {

        string name(n->name());

        if (++aux[name] > max) {
        
            exception_message << "XMLParser: Expected at max " << std::to_string(max) << ", but got " << std::to_string(aux[name]) << " in node '" << this->name() << "'.";
            throw InvalidXMLStructure(exception_message.str());
        }
    }
}

vector<XMLParser> XMLParser::get_nodes() {
    vector<XMLParser> ans;
    
    for (xml_node<> *n = node->first_node(); n; n = n->next_sibling())
        ans.push_back(XMLParser(content, doc, n));

    return ans;
}

vector<XMLParser> XMLParser::get_nodes(string name) {
    vector<XMLParser> ans;
    
    for (xml_node<> *n = node->first_node(); n; n = n->next_sibling())
        if (string(n->name()) == name)
            ans.push_back(XMLParser(content, doc, n));

    return ans;
}

XMLParser XMLParser::get_node(string name) {

    std::stringstream exception_message;

    for (xml_node<> *n = node->first_node(); n; n = n->next_sibling())
        if (string(n->name()) == name) 
            return XMLParser(content, doc, n);

    exception_message << "XMLParser: The child node '" << name << "' of node '" << this->name() << "' does not exist.";
    throw InvalidXMLStructure(exception_message.str());
}

void XMLParser::validate_attrs(initializer_list<string> attrs) {

    set<string> aux(attrs);
    std::stringstream exception_message;
    
    for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())

        if (aux.find(string(attr->name())) == aux.end()) {

            exception_message << "XMLParser: The attribute '" << attr->name() << "' is not valid for the node '" << name() << "'.";
            throw InvalidXMLStructure(exception_message.str());

        }
}
