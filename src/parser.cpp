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

#include "rapidxml.h"
#include "../include/config.hpp"
#include "../include/parser.hpp"
#include "../include/exceptions/invalid_xml_file.hpp"

using namespace std;
using namespace rapidxml;

XMLParser::XMLParser(std::string file_path) {

    ifstream test(file_path);

    if (!test)
        throw InvalidXMLStructure("engine:XMLParser : The XML file provided does not exits.");

    this->file_path = std::move(file_path);
}

World XMLParser::parse() {

    /* Setting up the XML parser with the file. */
    xml_document<> doc;

    ifstream file(this->file_path);
    stringstream buffer;

    buffer << file.rdbuf();
    file.close();

    string content(buffer.str());
    doc.parse<0>(&content[0]);

    /* Getting the root node on the XML file. Must be 'world'. */
    xml_node<> *root_node = doc.first_node();

    if (strcmp(root_node->name(), "world") != 0) {
        throw InvalidXMLStructure("engine:parse : The XML file should contain a 'world' root node.");
    }

    /* Parsing the 'window', 'camera' and 'group' nodes. */
    WindowSize window = XMLParser::parse_window(root_node);
    Camera camera = XMLParser::parse_camera(root_node);
    Models models = XMLParser::parse_group(root_node);

    return {window, camera, models};
}

WindowSize XMLParser::parse_window(xml_node<> *root_node) {

    WindowSize window;

    xml_node<> *window_node = root_node->first_node("window");

    for (xml_attribute<> *attr = window_node->first_attribute();
         attr; attr = attr->next_attribute()) /* Iterate over its attributes and store them. */
    {
        if (strcmp(attr->name(), "width") == 0) {
            get<0>(window) = stoi(attr->value()); /* 0-index is the width. */
        }

        else if (strcmp(attr->name(), "height") == 0) {
            get<1>(window) = stoi(attr->value()); /* 1-index is the height. */
        }

        else throw InvalidXMLStructure("engine:parse_window : The 'WindowSize' node should only have the attributes 'width' and 'height'.");
    }

    return window;
}

Camera XMLParser::parse_camera(xml_node<> *root_node) {

    Camera camera;
    Point position, look_at, up, projection;

    xml_node<> *camera_node = root_node->first_node("camera");

    xml_node<> *position_node = camera_node->first_node("position");
    xml_node<> *look_at_node = camera_node->first_node("lookAt");
    xml_node<> *up_node = camera_node->first_node("up");
    xml_node<> *projection_node = camera_node->first_node("projection");

    if (!position_node || !look_at_node || !up_node || !projection_node)
        throw InvalidXMLStructure("engine:parse_camera : The 'camera' node has to have the nodes 'position', 'lookAt', 'up' and 'projection'.");

    position = XMLParser::get_attr_as_point(position_node, "x", "y", "z");
    look_at = XMLParser::get_attr_as_point(look_at_node, "x", "y", "z");
    up = XMLParser::get_attr_as_point(up_node, "x", "y", "z");
    projection = XMLParser::get_attr_as_point(projection_node, "fov", "near", "far");

    /*
    cout << get<0>(position) << " " << get<1>(position) << " " << get<2>(position) << "\n";
    cout << get<0>(look_at) << " " << get<1>(look_at) << " " << get<2>(look_at) << "\n";
    cout << get<0>(up) << " " << get<1>(up) << " " << get<2>(up) << "\n";
    cout << get<0>(projection) << " " << get<1>(projection) << " " << get<2>(projection) << "\n";
    */

    return {position, look_at, up, projection};

}

Point XMLParser::get_attr_as_point(xml_node<> *node, const string& label_a, const string& label_b, const string& label_c) {

    Point result;

    for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {

        string label = attr->name();

        if (label == label_a) get<0>(result) = stof(attr->value());
        else if (label == label_b) get<1>(result) = stof(attr->value());
        else if (label == label_c) get<2>(result) = stof(attr->value());
        else throw InvalidXMLStructure("engine:get_attr_as_point : Invalid attributes inside 'camera' nodes.");
    }

    return result;
}

Models XMLParser::parse_group(rapidxml::xml_node<> *root_node) {

    vector<Shape> result;

    xml_node<> *group_node = root_node->first_node("group");
    if (group_node == nullptr) throw InvalidXMLStructure("engine:parse_group : The file must have a 'group' node.");

    xml_node<> *models_node = group_node->first_node("models");
    if (models_node == nullptr) throw InvalidXMLStructure("engine:parse_group : The file must have a 'models' node inside the 'group' node");

    for (xml_node<> *model_node = models_node->first_node(); model_node; model_node = model_node->next_sibling()) {

        string node_name = model_node->name();
        if (node_name != "model") throw InvalidXMLStructure("engine:parse_group : Inside the 'models' can only be present 'model' nodes.");

        xml_attribute<> *attr = model_node->first_attribute();
        string label = attr->name();

        if (label != "file") throw InvalidXMLStructure("engine:parse_group : 'model' nodes can only have the attribute 'file'.");
        result.emplace_back(model_node->first_attribute()->value());
    }

    return result;
}
