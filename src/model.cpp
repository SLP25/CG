#include "glut.hpp"

#include <sstream>
#include <string>
#include <cstring>

#include <iostream>

#include "model.hpp"
#include "parser.hpp"

Model::Model(Shape shape, Color color) :
  shape(std::move(std::make_shared<Shape>(shape))),
  texture(std::shared_ptr<Texture>()),
  color(color)
{}

Model::Model(XMLParser parser) {
  /**
   * @brief parses the model atribute from the xml parser into a Model object
   * @param parser the model xml data
   */
  parser.validate_node({"texture", "color"});
  parser.validate_max_nodes(1, {"texture", "color"});
  parser.validate_attrs({"file", "color"}); //TODO: read color from its own xml tag

  this->shape = Shape::fetchShape(parser.get_attr<std::string>("file"));

  if (parser.get_nodes().size() != 0) {
    XMLParser textureNode = parser.get_node("texture");
    textureNode.validate_node({});
    textureNode.validate_attrs({"file"});
    this->texture = Texture::fetchTexture(textureNode.get_attr<std::string>("file"));
  } else {
    this->texture = nullptr;
  }

  //TODO: default colors (see xml format)
  std::string colorHex = "#ff0000";
  parser.get_opt_attr("color", colorHex);
  this->color = parseHexColor(colorHex);
}

void Model::draw() {
  /**
   * @brief Draws the model in the window
   * 
   */

  //glColor3f(GET_ALL(color));

	float white[] = { 0.8, 0.8, 0.8, 1.0 };
  float black[] = { 0.0, 0.0, 0.0, 1.0 };

  float aux[] = { GET_ALL(color), 1.0 };
  
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, aux);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);
  glMaterialfv(GL_FRONT, GL_EMISSION, black);

  if (texture != nullptr)
    texture->bind();
  else
    Texture::unbind();

  shape->draw();
}
