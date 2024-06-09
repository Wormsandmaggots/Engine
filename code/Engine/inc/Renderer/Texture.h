#pragma once
#include <cstdint>
#include <string>

#include <stb_image.h>
#include <glad/glad.h>

#include "Debug/Logger.h"
class Texture {
private:
public:
	uint32_t textureID;
	std::string textureName;
	std::string texturePath;


	explicit Texture(const std::string& path,const std::string& name);
	void bind();
	void unbind();

};