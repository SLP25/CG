#include "glut.hpp"

#include <sstream>
#include <string>
#include <cstring>

#include <iostream>

#include "model.hpp"
#include "parser.hpp"

Model::Model(Shape shape, Texture texture, Color emission, Color ambient, Color diffuse, Color specular, float shininess) :
  shape(std::move(std::shared_ptr<Shape>(new Shape(shape)))),
  texture(std::move(std::shared_ptr<Texture>(new Texture(texture)))),
  emission(emission),
  ambient(ambient),
  diffuse(diffuse),
  specular(specular),
  shininess(shininess)
{}

void Model::readColor(XMLParser color) {
  std::string colorHex;

  if (color.get_opt_attr("hex", colorHex)) {

    this->diffuse = parseHexColor(colorHex);

    if (color.get_nodes().size() != 0)
      throw InvalidXMLStructure("Can't define both hex and color values for a given color");
  } else {
    for (XMLParser node : color.get_nodes()) {
      node.validate_node({});
      node.validate_attrs({"R","G","B","value"});
      if (node.name() == "diffuse")
        diffuse = node.as_tuple<float,float,float>({"R","G","B"}) / 255.0f;
      else if (node.name() == "ambient")
        ambient = node.as_tuple<float,float,float>({"R","G","B"}) / 255.0f;
      else if (node.name() == "specular")
        specular = node.as_tuple<float,float,float>({"R","G","B"}) / 255.0f;
      else if (node.name() == "emissive")
        emission = node.as_tuple<float,float,float>({"R","G","B"}) / 255.0f;
      else //shininess
        shininess = node.get_attr<float>("value");
    }
  }
}

Model::Model(XMLParser parser) : 
  texture(nullptr),
  emission({0, 0, 0}),
  ambient({0.2, 0.2, 0.2}),
  diffuse({0.8, 0.8, 0.8}),
  specular({0, 0, 0}),
  shininess(0)
{
  /**
   * @brief parses the model atribute from the xml parser into a Model object
   * @param parser the model xml data
   */
  parser.validate_node({"texture", "color"});
  parser.validate_max_nodes(1, {"texture", "color"});
  parser.validate_attrs({"file"});

  this->shape = Shape::fetchShape(parser.get_attr<std::string>("file"));

  for (XMLParser node : parser.get_nodes()) {
    if (node.name() == "color")
    {
      node.validate_node({"diffuse", "ambient", "specular", "emissive", "shininess"});
      node.validate_max_nodes(1, {"diffuse", "ambient", "specular", "emissive", "shininess"});
      node.validate_attrs({"hex"});
      readColor(node);
    }
    else //texture
    {
      node.validate_node({});
      node.validate_attrs({"file"});
      this->texture = Texture::fetchTexture(node.get_attr<std::string>("file"));
    }
  }  
}

void Model::draw(const Frustum& viewFrustum)
{
  float modelview[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
  transpose(4, 4, modelview); //glut stores matrices in column-major order. we want row-major

  BoundingBox bb = shape->getBoundingBox();
  bb.transform(modelview);

  if (viewFrustum.contains(bb)) {
    //std::cout << "visible" << std::endl;
    float emi[] = { GET_ALL(emission), 1.0 };
    float amb[] = { GET_ALL(ambient), 1.0 };
    float dif[] = { GET_ALL(diffuse), 1.0 };
    float spec[] = { GET_ALL(specular), 1.0 };
    
    glMaterialfv(GL_FRONT, GL_EMISSION, emi);
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    if (texture != nullptr)
      texture->bind();
    else
      Texture::unbind();

    shape->draw();
  } //else {
    //std::cout << "invisible" << std::endl;
  //}
}
