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

    uint32_t getTextureID() const
    {
        return textureID;
    }

    Texture(const std::string& path)
    {
        textureName = "default";
        texturePath = path;

        int width, height, numberOfComponents;
        uint8_t* imageData = stbi_load(texturePath.c_str(), &width, &height, &numberOfComponents, 0);
        if (imageData) {
            int32_t storageColorFormat;
            int32_t imageColorFormat;
            switch (numberOfComponents) {
                case 1:
                    imageColorFormat = GL_RED;
                    storageColorFormat = GL_RED;
                    break;
                case 3:
                    imageColorFormat = GL_RGB;
                    storageColorFormat = GL_RGB8;
                    break;
                case 4:
                    imageColorFormat = GL_RGBA;
                    storageColorFormat = GL_RGBA8;
                    break;
                default:
                    LOG_INFO("Texture format not implemented");
            }

            glCreateTextures(GL_TEXTURE_2D, 1, &textureID);

            glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTextureStorage2D(textureID, 1, storageColorFormat, width, height);
            glTextureSubImage2D(textureID, 0, 0, 0, width, height, imageColorFormat, GL_UNSIGNED_BYTE, imageData);
            glGenerateTextureMipmap(textureID);
            stbi_image_free(imageData);
        }
        else {
            SPDLOG_ERROR("Failed to load texture at path: {}", path);
            stbi_image_free(imageData);
        }
    }
};