/**
 * @file parser.hpp
 * @brief Definition of the XMLParser class.
 */

#pragma once

#include <memory>
#include <sstream>
#include <string>
#include "utils.hpp"
#include "model.hpp"
#include <rapidxml/rapidxml.hpp>
#include "exceptions/invalid_xml_file.hpp"

using namespace std;
using namespace rapidxml;

typedef initializer_list<string> attrs;

class XMLParser;

template<typename... Ts> struct aux {
    static tuple<Ts...> as_tuple(XMLParser& parser, attrs& attrs, attrs::iterator a);
    static tuple<Ts...> as_tuple_opt(XMLParser& parser, attrs& attrs, attrs::iterator a);
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

    string name();

    //throws exception if a child node is found with a name not in names
    void validate_node(attrs names);

    //throws exception if more than max child nodes with a name in names are found
    void validate_max_nodes(int max, attrs names);

    XMLParser get_node(string name);
    vector<XMLParser> get_nodes();
    vector<XMLParser> get_nodes(string name);

    //throws exception if an attribute with a name not specified in attrs is found
    void validate_attrs(attrs attrs);

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

    template<typename T> bool get_opt_attr(string name, T& ans) {
        for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
            if (string(attr->name()) == name) {
                stringstream convert(attr->value());
                convert >> ans;
                return true;
            }
        }

        return false;
    }

    template<typename... Ts> tuple<Ts...> as_tuple(attrs attrs) {
        return aux<Ts...>::as_tuple(*this, attrs, attrs.begin());
    }
    
    template<typename... Ts> tuple<Ts...> as_tuple_opt(attrs attrs) {
        return aux<Ts...>::as_tuple_opt(*this, attrs, attrs.begin());
    }

    template<typename T, typename... Args> T as_object(attrs attrs) {
        return make_from_tuple<T>(as_tuple<Args...>(attrs));
    }

    template<typename T, typename... Args> T as_object_opt(attrs attrs) {
        return make_from_tuple<T>(as_tuple_opt<Args...>(attrs));
    }

private:
    XMLParser(shared_ptr<string> content, shared_ptr<xml_document<>> doc, xml_node<> *node);
};

template<typename T, typename... Ts> struct aux<T, Ts...> {
    static tuple<T,Ts...> as_tuple(XMLParser& parser, attrs& attrs, attrs::iterator a) {      
        if (a == attrs.end())
            throw InvalidXMLStructure("as_tuple: Too few arguments in initializer list.");
        
        T head = parser.get_attr<T>(*a);
        tuple<Ts...> tail = aux<Ts...>::as_tuple(parser, attrs, a + 1);

        return tuple_cat(make_tuple(head), tail);
    }

    static tuple<T,Ts...> as_tuple_opt(XMLParser& parser, attrs& attrs, attrs::iterator a) {      
        if (a == attrs.end())
            throw InvalidXMLStructure("as_tuple_opt: Too few arguments in initializer list.");
        
        T head;
        tuple<Ts...> tail = aux<Ts...>::as_tuple_opt(parser, attrs, a + 1);

        if (parser.get_opt_attr(*a, head)) {
            return tuple_cat(make_tuple(head), tail);
        } else {
            return tail;
        }
    }
};

template<> struct aux<> {
    static tuple<> as_tuple(__attribute__((unused)) XMLParser&, attrs& attrs, attrs::iterator a) {
        if (a != attrs.end())
            throw InvalidXMLStructure("as_tuple: Too many arguments in initializer list.");
        
        return { };
    }

    static tuple<> as_tuple_opt(__attribute__((unused)) XMLParser&, attrs& attrs, attrs::iterator a) {
        if (a != attrs.end())
            throw InvalidXMLStructure("as_tuple_opt: Too many arguments in initializer list.");
        
        return { };
    }
};
