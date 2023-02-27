/**
 * @file parser.hpp
 * @brief Definition of the XMLParser class.
 */

#pragma once

#include "config.hpp"
#include <rapidxml/rapidxml.hpp>
#include <string>

/**
 * @brief A class representing a XML parser using rapidxml lib.
 */
class XMLParser {

  std::string file_path;

public:
  /**
   * Default parser constructor.
   * @param file_path Path of the XML file to be parsed.
   */
  explicit XMLParser(std::string file_path);

  /**
   * Parses the file passed on the constructor.
   * @return World object with the parsed information.
   */
  World parse();

private:
  /**
   * Parses the 'window' node section.
   * @param root_node Main node of the XML file ('world').
   * @return WindowSize object with the information of the width (0) and height
   * (1).
   */
  static WindowSize parse_window(rapidxml::xml_node<> *root_node);

  /**
   * Parses the 'camera' node section.
   * @param root_node Main node of the XML file ('world').
   * @return Camera object with the information of the points Position, LookAt,
   * Up and Projection.
   */
  static Camera parse_camera(rapidxml::xml_node<> *root_node);

  /**
   * Parses the 'group' node section (containing the models).
   * @param root_node Main node of the XML file ('world').
   * @return Models object with the information of the models parsed.
   */
  static Models parse_group(rapidxml::xml_node<> *root_node);

  /**
   * Get a point from a node with three attributes indicating a point.
   * @param node Node to parse the attributes.
   * @param label_a X coordinate label.
   * @param label_b Y coordinate label.
   * @param label_c Z coordinate label.
   * @return Point struct.
   */
  static Point get_attr_as_point(rapidxml::xml_node<> *node,
                                 const std::string &label_a,
                                 const std::string &label_b,
                                 const std::string &label_c);
};
