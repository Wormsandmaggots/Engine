#pragma once
#include <cstdint>
#include <string>

#include <stb_image.h>
#include <glad/glad.h>

#include "Debug/Logger.h"
class Texture {
private:
public:
	uint32_t textureID = 0;
	std::string textureName;
	std::string texturePath;


	explicit Texture(const std::string& path,const std::string& name);
	~Texture();
	void bind();
	void unbind();
};