#pragma once

/**
 * @file group.hpp
 * @brief File declaring the Group class, used to hierarchically render objects
*/

#include "model.hpp"
#include "parser.hpp"
#include "transformation.hpp"

/**
 *  @brief A class used to hierarchically render objects. A group is a tree-like
 * structure, which can have subgroups inside it. It contains informations on 
 * transformations to apply to the scene as well as the models to render. When 
 * each subgroup is rendered, all transformations applied to the current group
 * persist, and new ones are, as a result, composed with them
*/
class Group {
public:
  /**
   * @brief Default empty constructor
  */
  Group();

  /**
   * @brief Copy constructor
   * 
   * @param group the Group to copy
  */
  Group(const Group& group);

  /**
   * @brief Move constructor
   * 
   * @param group The Group to move
  */
  Group(Group&& group);

  /**
   * @brief Constructs a Group based on an XML string
   * 
   * @param parser the parser of the XML string corresponding to the group
  */ 
  Group(XMLParser parser);

  /**
   * @brief Assignment operator
   * @param group the Group to assign this as
  */
  Group& operator=(const Group& group);

  /**
   * @brief Assignment operator
   * @param group the Group to assign this as
  */
  Group& operator=(Group&& other);

  /**
   * @brief Renders the group to the screen.
   * 
   * This method does not change GLUT's matrices - i.e. they will be the same
   * before and after execution.
  */
  void draw(const Frustrum& viewFrustrum);

private:
  /**
   * @brief asserts the validity of the XML string.
   * 
   * Auxiliary method for the XML constructor
   * 
   * @param parser the parser for the XML string
   * 
   * @throws If the string is not valid, an #InvalidXMLStructure exception
   * is thrown
  */
  inline void assertValidXML(XMLParser parser);

  /**
   * @brief Recursively constructs the subgroups based on the XML string
   * 
   * Auxiliary method for the XML constructor
   * 
   * @param parser the parser for the XML string
   * 
   * @throws If the string is not valid, an #InvalidXMLStructure exception
   * is thrown
  */
  inline void constructSubGroups(XMLParser parser);

  /**
   * @brief Builds the models based on the XML string.
   * 
   * Auxiliary method for the XML constructor
   * 
   * @param parser the parser for the XML string
   * 
   * @throws If the string is not valid, an #InvalidXMLStructure exception
   * is thrown
  */
  inline void constructModels(XMLParser parser);

  /**
   * @brief Builds the transformations to apply to the group based on the XML string.
   * 
   * Auxiliary method for the XML constructor
   * 
   * @param parser the parser for the XML string
   * 
   * @throws If the string is not valid, an #InvalidXMLStructure exception
   * is thrown
  */
  inline void constructTransformations(XMLParser parser);

private:
  /**
   * @brief The subgroups of the group
  */
  std::vector<Group> subgroups;
  /**
   * @brief The models of the group
  */
  std::vector<Model> models;

  /**
   * @brief The transformations to apply to this group
  */
  std::vector<std::unique_ptr<Transformation>> transformations;
};
