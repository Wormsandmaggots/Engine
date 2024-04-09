#ifndef ENGINE_IMAGE_H
#define ENGINE_IMAGE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "stb_image.h"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

class Image {
private:
    unsigned int VAO, VBO, EBO, texture;

protected:
    Shader shader;

    void updateVertices(const std::vector<float>& vertices);
    void setColor(const glm::vec3& color);

public:
    Image(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& texturePath);
    void render();
    ~Image();
};

#endif