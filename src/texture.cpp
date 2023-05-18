#include "texture.hpp"
#include "glut.hpp"
#include <map>
#include <IL/il.h>

std::map<std::string,std::shared_ptr<Texture>> Texture::cache;

std::shared_ptr<Texture> Texture::fetchTexture(std::string filePath) {

    if (cache.find(filePath) != cache.end())
        return cache[filePath];

    std::shared_ptr<Texture> t = std::shared_ptr<Texture>(new Texture(filePath));
    cache[filePath] = t;
    return t;
}

void Texture::initTextures() {
    for (const auto& t : cache)
        t.second->initialize();
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(std::string filePath) : texture(0) {
    //TODO: meter numa inicialização global?
    ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    unsigned int t;
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)filePath.c_str());
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	data = ilGetData();
}

Texture::~Texture() {
    if (texture != 0)
        glDeleteTextures(1, &texture);
}

void Texture::initialize() {
    glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::bind() {
    if (texture == 0)
        throw std::runtime_error("Attept to bind uninitialized texture");

    glBindTexture(GL_TEXTURE_2D, texture);
}