/**
 * @file parser.hpp
 * @brief Definition of the XMLParser class.
 */

#pragma once

#include "model.hpp"
#include "utils.hpp"
#include <memory>
#include <sstream>
#include <string>

#ifdef FEDORA
#include "rapidxml.h"
#else
#include <rapidxml/rapidxml.hpp>
#endif

#include "exceptions/invalid_xml_file.hpp"

typedef std::initializer_list<std::string> attrs;

class XMLParser;

template <typename... Ts> struct aux {
  static std::tuple<Ts...> as_tuple(XMLParser &parser, attrs &attrs,
                                    attrs::iterator a);
  static std::tuple<Ts...> as_tuple_opt(XMLParser &parser, attrs &attrs,
                                        attrs::iterator a);
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
   * Default parser constructor.
   * @param file_path Path of the XML file to be parsed.
   */
  explicit XMLParser(std::string file_path);

  std::string name();

  // throws exception if a child node is found with a name not in names
  void validate_node(attrs names);

  // throws exception if more than max child nodes with a name in names are
  // found
  void validate_max_nodes(int max, attrs names);

  XMLParser get_node(std::string name);
  std::vector<XMLParser> get_nodes();
  std::vector<XMLParser> get_nodes(std::string name);

  // throws exception if an attribute with a name not specified in attrs is
  // found
  void validate_attrs(attrs attrs);

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

  template <typename... Ts> std::tuple<Ts...> as_tuple(attrs attrs) {
    return aux<Ts...>::as_tuple(*this, attrs, attrs.begin());
  }

  template <typename... Ts> std::tuple<Ts...> as_tuple_opt(attrs attrs) {
    return aux<Ts...>::as_tuple_opt(*this, attrs, attrs.begin());
  }

  template <typename T, typename... Args> T as_object(attrs attrs) {
    return std::make_from_tuple<T>(as_tuple<Args...>(attrs));
  }

  template <typename T, typename... Args> T as_object_opt(attrs attrs) {
    return std::make_from_tuple<T>(as_tuple_opt<Args...>(attrs));
  }

private:
  XMLParser(std::shared_ptr<std::string> content,
            std::shared_ptr<rapidxml::xml_document<>> doc,
            rapidxml::xml_node<> *node);
};

template <typename T, typename... Ts> struct aux<T, Ts...> {
  static std::tuple<T, Ts...> as_tuple(XMLParser &parser, attrs &attrs,
                                       attrs::iterator a) {
    if (a == attrs.end())
      throw InvalidXMLStructure(
          "as_tuple: Too few arguments in initializer list.");

    T head = parser.get_attr<T>(*a);
    std::tuple<Ts...> tail = aux<Ts...>::as_tuple(parser, attrs, a + 1);

    return tuple_cat(std::make_tuple(head), tail);
  }

  static std::tuple<T, Ts...> as_tuple_opt(XMLParser &parser, attrs &attrs,
                                           attrs::iterator a) {
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
  static std::tuple<> as_tuple(__attribute__((unused)) XMLParser &,
                               attrs &attrs, attrs::iterator a) {
    if (a != attrs.end())
      throw InvalidXMLStructure(
          "as_tuple: Too many arguments in initializer list.");

    return {};
  }

  static std::tuple<> as_tuple_opt(__attribute__((unused)) XMLParser &,
                                   attrs &attrs, attrs::iterator a) {
    if (a != attrs.end())
      throw InvalidXMLStructure(
          "as_tuple_opt: Too many arguments in initializer list.");

    return {};
  }
};
