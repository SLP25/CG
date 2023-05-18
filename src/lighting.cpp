#include "glut.hpp"
#include "lighting.hpp"
#include <memory>

Lighting::Lighting() {}

Lighting::Lighting(const Lighting& lighting) {

    //TODO: ?
    float black[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);

    for (auto& l : lighting.lights) {
        Light* copy = l->clone();
        std::unique_ptr<Light> ptr(copy);
        this->lights.push_back(std::move(ptr));
    }
        
}

Lighting& Lighting::operator=(const Lighting& lighting) {
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
    parser.validate_max_nodes(8, {"light"});

    for (const XMLParser& x : parser.get_nodes("light"))
        this->lights.push_back(Light::parse(x));
}

void Lighting::initScene() {
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glEnable(GL_RESCALE_NORMAL);
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
}

Light* PointLight::clone() {
    return new PointLight(*this);
}

float dark[4] = {0.2, 0.2, 0.2, 1.0};
float white[4] = {1.0, 1.0, 1.0, 1.0};
float black[4] = {0.0f, 0.0f, 0.0f, 0.0f};

void PointLight::draw(int index) {
    //TODO: color of light??
	glLightfv(GL_LIGHT0 + index, GL_AMBIENT, dark);
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0 + index, GL_SPECULAR, white);

    float aux[4] = { GET_ALL(pos), 1 };
    glLightfv(GL_LIGHT0 + index, GL_POSITION, aux);
}


bool DirectionalLight::accepts(XMLParser parser) {
    return parser.name() == "light" && parser.get_attr<std::string>("type") == "directional";
}

DirectionalLight::DirectionalLight(XMLParser parser) {
    parser.validate_attrs({"type", "dirX", "dirY", "dirZ"});
    parser.validate_node({});
    this->direction = parser.as_tuple<float,float,float>({"dirX", "dirY", "dirZ"});
}

Light* DirectionalLight::clone() {
    return new DirectionalLight(*this);
}

void DirectionalLight::draw(int index) {
    glLightfv(GL_LIGHT0 + index, GL_AMBIENT, dark);
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0 + index, GL_SPECULAR, white);

    float aux[4] = { GET_ALL(direction), 0 };
    glLightfv(GL_LIGHT0 + index, GL_POSITION, aux);
}


