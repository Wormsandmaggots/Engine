#pragma once
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include "Shader.h"
#include "Renderer/Texture.h"
#include <nlohmann/json.hpp>


using json = nlohmann::json;

class Material {
    struct Uniform {
        std::string name;
        virtual void ApplyValue(Shader* shader) = 0;
        virtual std::unique_ptr<Uniform> Clone() = 0;

        virtual ~Uniform() = default;
    };

    struct TextureUniform: public Uniform {
        int index;
        Texture* textureAsset;
        void ApplyValue( Shader* shader) override;

        std::unique_ptr<Uniform> Clone() override;
    };
    struct Vec4Uniform : public Uniform {
        glm::vec4 value{};
        void ApplyValue(Shader* shader) override;

        std::unique_ptr<Uniform> Clone() override;
    };

    struct FloatUniform : public Uniform {
        float value{};
        void ApplyValue(Shader* shader) override;
        std::unique_ptr<Uniform> Clone() override;
    };

    struct IntUniform : public Uniform {
        int value{};
        void ApplyValue(Shader* shader) override;
        std::unique_ptr<Uniform> Clone() override;
    };

    std::string materialPath;
    Shader* shader;
    std::vector<std::unique_ptr<Uniform>> uniforms;

public:
    Material(const std::string& path);
    void SetFloat(const std::string& name, float value);
    void SetInt(const std::string& name, int value);
    void SetVec4(const std::string& name, glm::vec4 value);
    void SetTexture(
        const std::string& name,
        Texture* texture);
    void bindMaterial();
    void unbindMaterial();
    void RemoveUniform(const std::string& name);
    void ParseTextures(const json& materialJson);
    void ParseUniforms(const json& materialJson);
    Shader* getShader();
};
