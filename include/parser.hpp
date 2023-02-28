/**
 * @file parser.hpp
 * @brief Definition of the XMLParser class.
 */

#pragma once

#include <memory>
#include <sstream>
#include <string>
#include "config.hpp"
#include "model.hpp"
#include <rapidxml/rapidxml.hpp>
#include "exceptions/invalid_xml_file.hpp"

using namespace std;
using namespace rapidxml;

class XMLParser;

template<typename... Ts> struct aux {
    static tuple<Ts...> as_tuple(XMLParser* parser, initializer_list<string>::iterator attrs);
};

/**
 * @brief A class representing a XML parser using rapidxml lib.
 */
class XMLParser {
    shared_ptr<string> content;
    shared_ptr<xml_document<>> doc;
    xml_node<> *node;

public:
    /**
     * Default parser constructor.
     * @param file_path Path of the XML file to be parsed.
     */
    explicit XMLParser(string file_path);

    XMLParser get_node(string name);
    vector<XMLParser> get_nodes();
    vector<XMLParser> get_nodes(string name);


    template<typename T> T get_attr(string name) {
        for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
            if (string(attr->name()) == name) {
                stringstream convert(attr->value());
                T value;
                convert >> value;
                return value;
            }
        }

        throw InvalidXMLStructure("engine:get_attr : No such attribute exists in the node.");
    }

    template<typename... Ts> tuple<Ts...> as_tuple(initializer_list<string> attrs) {
        return aux<Ts...>::as_tuple(this, attrs.begin());
    }

    template<typename T, typename... Args> T as_object(initializer_list<string> attrs) {
        return make_from_tuple<T>(as_tuple<Args...>(attrs));
    }

private:
    XMLParser(shared_ptr<string> content, shared_ptr<xml_document<>> doc, xml_node<> *node);
};

template<typename T, typename... Ts> struct aux<T, Ts...> {
    static tuple<T,Ts...> as_tuple(XMLParser* parser, initializer_list<string>::iterator attrs) {      
        T head = parser->get_attr<T>(*attrs);
        tuple<Ts...> tail = aux<Ts...>::as_tuple(parser, attrs + 1);

        return tuple_cat(make_tuple(head), tail);
    }
};

template<> struct aux<> {
    static tuple<> as_tuple(__attribute__((unused)) XMLParser*, __attribute__((unused)) initializer_list<string>::iterator) {
        return { };
    }
};