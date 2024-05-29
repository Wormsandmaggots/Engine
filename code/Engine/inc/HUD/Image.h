#ifndef ENGINE_IMAGE_H
#define ENGINE_IMAGE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "stb_image.h"
#include "glm/gtc/matrix_transform.hpp"
#include "ECS/Component.h"
#include "Renderer/Renderable.h"
#include <vector>

class Image : public Component, public Renderable {
private:
    unsigned int VAO, VBO, EBO, texture;
    Transform2* transform;

protected:
    Shader shader;

    void updateVertices(const std::vector<float>& vertices);
    void setColor(const glm::vec3& color);

public:
    Image(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& texturePath);
    void render();
    void update() override;
    void awake() override {};
    void start() override {};
    Shader * getShader() override;
    Transform2 * getTransform() override;
    void setTransform(Transform2 *t) override;
    void onDestroy() override {};
    void Draw(Shader *shader) override;
    ~Image();
};

#endif