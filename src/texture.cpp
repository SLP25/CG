#include "texture.hpp"
#include "glut.hpp"
#include <map>
#include <cstring>
#include <IL/il.h>

std::map<std::string,std::shared_ptr<Texture>> Texture::cache;

std::shared_ptr<Texture> Texture::fetchTexture(std::string filePath) {

    if (cache.find(filePath) != cache.end())
        return cache[filePath];

    std::shared_ptr<Texture> t = std::shared_ptr<Texture>(new Texture(filePath));
    cache[filePath] = t;
    return t;
}

void Texture::clearCache() {
    cache.clear();
}

void Texture::initTextures() {
    for (const auto& t : cache)
        t.second->initialize();
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(std::string filePath) : texture(0) {
    ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    unsigned int t;
	ilGenImages(1, &t);
	ilBindImage(t);

    if (!ilLoadImage((ILstring)filePath.c_str())) {
        throw InvalidXMLStructure("Texture file '" + filePath + "' doesn't exist");
    }
	
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
    int n = width * height * 4;
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	unsigned char* temp = ilGetData();
    data = new unsigned char[n];
    memcpy(data, temp, n);
}

Texture::Texture(const Texture& texture) :
    width(texture.width),
    height(texture.height),
    texture(0)
{
    int n = width * height * 4;
    data = new unsigned char[n];
    memcpy(data, texture.data, n);
}

Texture::Texture(Texture&& texture) :
    width(texture.width),
    height(texture.height),
    data(texture.data),
    texture(texture.texture)
{}

Texture::~Texture() {
    if (texture != 0)
        glDeleteTextures(1, &texture);

    delete[] data;
}

Texture& Texture::operator=(const Texture& texture) {
    this->width = texture.width;
    this->height = texture.height;

    delete[] data;
    int n = width * height * 4;
    this->data = new unsigned char[n];
    memcpy(data, texture.data, n);

    this->texture = 0;
    return *this;
}

Texture& Texture::operator=(Texture&& texture) {
    this->width = texture.width;
    this->height = texture.height;
    delete[] data;
    this->data = texture.data;
    this->texture = texture.texture;

    return *this;
}


void Texture::initialize() {
    glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    GLint aux = gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,width,height,GL_RGBA,GL_UNSIGNED_BYTE,data);
    if (aux != 0)
        throw std::runtime_error("Error creating mipmap: " + std::string((char*)gluErrorString(aux)));
}

void Texture::bind() {
    if (texture == 0)
        throw std::runtime_error("Attept to bind uninitialized texture");

    glBindTexture(GL_TEXTURE_2D, texture);
}