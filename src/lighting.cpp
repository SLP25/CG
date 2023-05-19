#include "glut.hpp"
#include "lighting.hpp"
#include <memory>

Lighting::Lighting() {}

Lighting::Lighting(const Lighting& lighting) {
    this->ambient = lighting.ambient;

    for (auto& l : lighting.lights) {
        Light* copy = l->clone();
        std::unique_ptr<Light> ptr(copy);
        this->lights.push_back(std::move(ptr));
    }
        
}

Lighting& Lighting::operator=(const Lighting& lighting) {
    this->ambient = lighting.ambient;

    for (auto& l : lighting.lights) {
        Light* copy = l->clone();
        std::unique_ptr<Light> ptr(copy);
        this->lights.push_back(std::move(ptr));
    }

    return *this;
}

Lighting::Lighting(XMLParser parser) {
    parser.validate_attrs({});
    parser.validate_node({"light"});
    parser.validate_max_nodes(GL_MAX_LIGHTS, {"light"});

    for (const XMLParser& x : parser.get_nodes("light"))
        this->lights.push_back(Light::parse(x));

    this->ambient =  { 0.5, 0.5, 0.5 };
}

void Lighting::initScene() {
    float aux[4] = { GET_ALL(this->ambient), 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, aux);
}

void Lighting::setupScene() {
    int i = 0;
    for (std::unique_ptr<Light>& l : this->lights)
        l->draw(i++);
}

std::unique_ptr<Light> Light::parse(XMLParser parser) {
    return dynamicParser<Light, PointLight, DirectionalLight/*, SpotLight*/>::parse(parser);
}

Light::~Light() {}


bool PointLight::accepts(XMLParser parser) {
    return parser.name() == "light" && parser.get_attr<std::string>("type") == "point";
}

PointLight::PointLight(XMLParser parser) {
    parser.validate_attrs({"type", "posx", "posy", "posz"});
    parser.validate_node({});
    this->pos = parser.as_tuple<float,float,float>({"posx", "posy", "posz"});
    this->difuse = { 1, 1, 1 };
    this->specular = { 1, 1, 1 };
}

Light* PointLight::clone() {
    return new PointLight(*this);
}

void PointLight::draw(int index) {
    float dif[4] = { GET_ALL(difuse), 1 };
    float spec[4] = { GET_ALL(specular), 1 };
    float aux[4] = { GET_ALL(pos), 1 };

	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0 + index, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT0 + index, GL_POSITION, aux);
}


bool DirectionalLight::accepts(XMLParser parser) {
    return parser.name() == "light" && parser.get_attr<std::string>("type") == "directional";
}

DirectionalLight::DirectionalLight(XMLParser parser) {
    parser.validate_attrs({"type", "dirx", "diry", "dirz"});
    parser.validate_node({});
    this->direction = parser.as_tuple<float,float,float>({"dirx", "diry", "dirz"});
    this->difuse = { 1, 1, 1 };
    this->specular = { 1, 1, 1 };
}

Light* DirectionalLight::clone() {
    return new DirectionalLight(*this);
}

void DirectionalLight::draw(int index) {
    float dif[4] = { GET_ALL(difuse), 1 };
    float spec[4] = { GET_ALL(specular), 1 };
    float pos[4] = { GET_ALL(direction), 0 };

	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0 + index, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT0 + index, GL_POSITION, pos);
}


