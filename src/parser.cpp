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
    return doc->name();
}

void XMLParser::validate_node(initializer_list<string> names) {
    set<string> aux(names);

    for (xml_node<> *n = node->first_node(); n; n = n->next_sibling())
        if (aux.find(string(n->name())) == aux.end())
            throw InvalidXMLStructure("Illegal xml node found");
}

void XMLParser::validate_max_nodes(int max, initializer_list<string> names) {
    std::map<string,int> aux;

    for (string name : names)
        aux[name] = 0;

    for (xml_node<> *n = node->first_node(); n; n = n->next_sibling()) {
        string name(n->name());

        if (++aux[name] > max)
            throw InvalidXMLStructure("More than max xml nodes found");
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
    for (xml_node<> *n = node->first_node(); n; n = n->next_sibling())
        if (string(n->name()) == name)
            return XMLParser(content, doc, n);

    throw InvalidXMLStructure("No child node exists with the specified name");
}

void XMLParser::validate_attrs(initializer_list<string> attrs) {
    set<string> aux(attrs);
    
    for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
        if (aux.find(string(attr->name())) == aux.end())
            throw InvalidXMLStructure("Illegal argument found in xml node");
}