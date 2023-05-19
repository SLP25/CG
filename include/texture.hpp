#pragma once

#include "utils.hpp"
#include <map>
#include "glut.hpp"

class Texture {
public:
    static std::shared_ptr<Texture> fetchTexture(std::string filePath);
    static void initTextures();
    static void unbind();


    Texture(std::string filePath);
    Texture(const Texture& texture);
    Texture(Texture&& texture);
    ~Texture();

    Texture& operator=(const Texture& texture);
    Texture& operator=(Texture&& texture);

    void initialize();
    void bind();

private:
    static std::map<std::string, std::shared_ptr<Texture>> cache;

    int width, height;
    unsigned char* data;

    GLuint texture;
};