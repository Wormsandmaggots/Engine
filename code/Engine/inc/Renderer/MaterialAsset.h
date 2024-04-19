#pragma once
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include "Shader.h"


class MaterialAsset {
    struct Uniform {
        std::string name;
        virtual void ApplyValue(class ShaderProgram* shaderProgram) = 0;
        virtual std::unique_ptr<Uniform> Clone() = 0;
        virtual ~Uniform() = default;
    };

    struct Texture {
        std::string name;
        int index;
        std::string path;
    };

    std::string materialPath;
    Shader* shader;
    std::vector<std::unique_ptr<Uniform>> uniforms;
    std::vector<Texture> textures;

public:
    MaterialAsset(const std::string& path);
};
