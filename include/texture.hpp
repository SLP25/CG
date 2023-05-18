#pragma once

#include "utils.hpp"
#include <map>
#include "glut.hpp"

class Texture {
public:
    static std::shared_ptr<Texture> fetchTexture(std::string filePath);
    static void initTextures();
    static void unbind();

    ~Texture();

    void initialize();
    void bind();

private:
    static std::map<std::string, std::shared_ptr<Texture>> cache;

    Texture(std::string filePath);

    int width, height;
    unsigned char* data;

    GLuint texture;
};