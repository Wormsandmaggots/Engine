#include "Renderer/Texture.h"

Texture::Texture(const std::string& path, const std::string& name)
{
    
    textureName = name;
    texturePath = path;

    
    //int width, height, numberOfComponents;
    /*stbi_set_flip_vertically_on_load(true);
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
    }*/
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        LOG_ERROR("Texture failed to load at path: "+path);
        stbi_image_free(data);
    }
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &textureID);
}
