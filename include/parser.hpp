/**
 * @file parser.hpp
 * @brief Definition of the XMLParser class.
 */

#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>

#ifdef FEDORA
#include "rapidxml.h"
#else
#include "../toolkits/rapidxml/rapidxml.hpp"
#endif

#include "exceptions/invalid_xml_file.hpp"

typedef std::initializer_list<std::string> params;

class XMLParser;

template <typename... Ts> struct aux {
  static std::tuple<Ts...> as_tuple(XMLParser &parser, params &attrs,
                                    params::iterator a);
  static std::tuple<Ts...> as_tuple_opt(XMLParser &parser, params &attrs,
                                        params::iterator a);
};

/**
 * @brief A class representing a XML parser using rapidxml lib.
 */
class XMLParser {
  std::shared_ptr<std::string> content;
  std::shared_ptr<rapidxml::xml_document<>> doc;
  rapidxml::xml_node<> *node;

public:
  /**
   * @brief Default parser constructor.
   * @param file_path Path of the XML file to be parsed.
   */
  explicit XMLParser(std::string file_path);

  /**
   * @brief Returns the name of the current node
   *
   * @return std::string
   */
  std::string name();

  /**
   * @brief Throws an error if any of the subnodes of the current node aren't in
   * the specified list of node names
   *
   * @param names The specified list
   */
  void validate_node(params names);

  /**
   * @brief Throws an error if any of the specified node names are found more
   * than max times
   *
   * @param max   The max number of allowed times for the nodes
   * @param names The names of subnodes to check
   */
  void validate_max_nodes(int max, params names);

  /**
   * @brief Returns an XMLParser object for the first subnode of the current
   * node with the specified name
   *
   * @param name The specified name
   * @return XMLParser The XMLParser object
   */
  XMLParser get_node(std::string name);

  /**
   * @brief Returns a vector of XMLParser objects corresponding to the subnodes
   * of the current node, in the order they appear in the file
   *
   * @return std::vector<XMLParser> A vector of XMLParser objects corresponding
   * to the subnodes of the current node, in the order they appear in the file
   */
  std::vector<XMLParser> get_nodes();

  /**
   * @brief Returns a vector of XMLParser objects corresponding to the subnodes
   * of the current node with the specified name, in the order they appear in
   * the file
   *
   * @return std::vector<XMLParser> A vector of XMLParser objects corresponding
   * to the subnodes of the current node with the specified name, in the order
   * they appear in the file
   */
  std::vector<XMLParser> get_nodes(std::string name);

  /**
   * @brief Throws an exception if any of the attributes of the current node
   * aren't in the list of allowed attributes
   *
   * Ex: <up x="0" y="1" z="0" w="0">.validate_node({"z, x, y"}) will throw an
   * error since w isn't on the list <up x="0" y="1" z="0">.validate_node({"x,
   * w, y, z"}) will not throw an error
   *
   * @param names The names of the allowed attributes for this node
   */
  void validate_attrs(params attrs);

  /**
   * @brief Gets the specified attribute of the current node, cast to the
   * specified type
   *
   * @tparam T    The type to cast the attribute to
   * @param name  The name of the attribute to retrieve
   * @return T    The attribute, cast to the specified type
   */
  template <typename T> T get_attr(std::string name) {
    for (rapidxml::xml_attribute<> *attr = node->first_attribute(); attr;
         attr = attr->next_attribute()) {
      if (std::string(attr->name()) == name) {
        std::stringstream convert(attr->value());
        T value;
        convert >> value;
        return value;
      }
    }

    throw InvalidXMLStructure(
        "engine:get_attr : No such attribute exists in the node.");
  }

  /**
   * @brief Optionally retrieves the specified attribute, cast to the given type
   *
   * @tparam T    The type to cast the attribute to
   * @param name  The name of the attribute to retrieve
   * @param ans   A reference to write the attribute, if found
   * @return true   The attribute exists in the current node and was written to
   * ans
   * @return false  The attribute doesn't exist
   */
  template <typename T> bool get_opt_attr(std::string name, T &ans) {
    for (rapidxml::xml_attribute<> *attr = node->first_attribute(); attr;
         attr = attr->next_attribute()) {
      if (std::string(attr->name()) == name) {
        std::stringstream convert(attr->value());
        convert >> ans;
        return true;
      }
    }

    return false;
  }

  /**
   * @brief Returns a tuple of the specified attributes, cast to the given types
   *
   * @tparam Ts   The types to cast the attributes to
   * @param attrs The names of the attributes
   * @return std::tuple<Ts...> A tuple containing the attributes
   */
  template <typename... Ts> std::tuple<Ts...> as_tuple(params attrs) {
    return aux<Ts...>::as_tuple(*this, attrs, attrs.begin());
  }

  /**
   * @brief Returns a tuple containing the existing attributes from among the
   * given list of attribute names, cast to the specified types. The types are
   * specified per given attribute, not per found attribute ex: <up x="0" y="1"
   * z="0">.as_tuple_opt<string, int, float>({"y", "w", "x"}) -> { "1", 0.0f }
   *
   * @tparam Ts
   * @param attrs
   * @return std::tuple<Ts...>
   */
  // template <typename... Ts> std::tuple<Ts...> as_tuple_opt(params attrs) {
  //   return aux<Ts...>::as_tuple_opt(*this, attrs, attrs.begin());
  // }

  /**
   * @brief Returns an object constructed from the specified attributes of the
   * current node. If no constructor of the given type accepts the specified
   * parameters, a compile-time error is thrown
   *
   * @tparam T    The type of the object to return
   * @tparam Args The arguments to the object constructor
   * @param attrs The attributes to fetch
   * @return T    The constructed object
   */
  template <typename T, typename... Args> T as_object(params attrs) {
    return std::make_from_tuple<T>(as_tuple<Args...>(attrs));
  }

  /**
   * @brief Returns an object constructed from the specified attributes of the
   * current node. If no constructor of the given type accepts the retrieved
   * parameters, an exception is thrown
   *
   * @tparam T    The type of the object to return
   * @tparam Args The arguments to the object constructor
   * @param attrs The attributes to fetch
   * @return T    The constructed object
   */
  // template <typename T, typename... Args> T as_object_opt(params attrs) {
  //   return std::make_from_tuple<T>(as_tuple_opt<Args...>(attrs));
  // }

private:
  /**
   * @brief Creates a XMLParser
   *
   * @param content The content of the xml file to parse
   * @param doc     The xml document, constructed from the content
   * @param node    The node of the XMLParser object
   */
  XMLParser(std::shared_ptr<std::string> content,
            std::shared_ptr<rapidxml::xml_document<>> doc,
            rapidxml::xml_node<> *node);
};

template <typename T, typename... Ts> struct aux<T, Ts...> {

    /**
    * @brief Takes a vector of XML attribute names and returns a tuple of
    * corresponding values.
    *
    * @param parser The XML parser.
    * @param attrs The vector of attribute names.
    * @param a The current iterator in the vector of attribute names.
    * @return std::tuple<T, Ts...> The tuple of corresponding values.
    */
  static std::tuple<T, Ts...> as_tuple(XMLParser &parser, params &attrs,
                                       params::iterator a) {
    if (a == attrs.end())
      throw InvalidXMLStructure(
          "as_tuple: Too few arguments in initializer list.");

    T head = parser.get_attr<T>(*a);
    std::tuple<Ts...> tail = aux<Ts...>::as_tuple(parser, attrs, a + 1);

    return tuple_cat(std::make_tuple(head), tail);
  }

    /**
     * @brief Converts an optional parameter list into a tuple.
     * @param parser The XML parser object used to extract parameter values.
     * @param attrs The parameter list to convert into a tuple.
     * @param a An iterator pointing to the current parameter being processed.
     * @return A tuple of parameter values.
     * @throws InvalidXMLStructure If the number of parameters does not match the number of types in the template parameters.
     */
  static std::tuple<T, Ts...> as_tuple_opt(XMLParser &parser, params &attrs,
                                           params::iterator a) {
    if (a == attrs.end())
      throw InvalidXMLStructure(
          "as_tuple_opt: Too few arguments in initializer list.");

    T head;
    std::tuple<Ts...> tail = aux<Ts...>::as_tuple_opt(parser, attrs, a + 1);

    if (parser.get_opt_attr(*a, head)) {
      return tuple_cat(make_tuple(head), tail);
    } else {
      return tail;
    }
  }
};

template <> struct aux<> {

    /**
   * @brief Converts an empty parameter list to an empty tuple
   * @param[in] parser XMLParser object to parse XML
   * @param[in] attrs parameters to be converted
   * @param[in] a iterator to the start of the parameter list
   * @return an empty tuple
   * @throws InvalidXMLStructure if there are too many arguments in the parameter list
   */
  static std::tuple<> as_tuple(XMLParser &,
                               params &attrs, params::iterator a) {
    if (a != attrs.end())
      throw InvalidXMLStructure(
          "as_tuple: Too many arguments in initializer list.");

    return {};
  }

    /**
     * @brief Converts an empty parameter list to an empty tuple
     * @param[in] parser XMLParser object to parse XML
     * @param[in] attrs parameters to be converted
     * @param[in] a iterator to the start of the parameter list
     * @return an empty tuple
     * @throws InvalidXMLStructure if there are too many arguments in the parameter list
     */
  static std::tuple<> as_tuple_opt(XMLParser &,
                                   params &attrs, params::iterator a) {
    if (a != attrs.end())
      throw InvalidXMLStructure(
          "as_tuple_opt: Too many arguments in initializer list.");

    return {};
  }
};
