#pragma once
#include <cstdint>
#include <string>

#include <stb_image.h>
#include <glad/glad.h>
#include <map>

#include "Debug/Logger.h"

enum TextureType{
    DIFFUSE,
    SPECULAR,
    METALLIC,
    NORMAL,
    AMBIENT,
    ROUGHNESS,
    DISPLACEMENT,
    EMISSIVE,
    EMISSIVECOLOR
};


class Texture {
private:
public:
	uint32_t textureID;
	std::string textureName;
    TextureType textureType;
	std::string texturePath;
    static std::map<std::string, TextureType> textureMap;


    explicit Texture(const std::string& path,const std::string& name);
	void bind();
	void unbind();
};