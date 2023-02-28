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

#include "config.hpp"
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