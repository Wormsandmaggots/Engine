#include "Renderer/Material.h"



Material::Material(const std::string& path) : materialPath(path)
{
    // Read and parse JSON file
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open JSON file: " + path);
    }

    json jsonData;
    try {
        file >> jsonData;
    }
    catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error("Error parsing JSON file: " + std::string(e.what()));
    }
    // Extract shader paths
    std::string vertexShaderPath = jsonData["vertexShader"];
    std::string fragmentShaderPath = jsonData["fragmentShader"];

    // Initialize shader (implementation of Shader class assumed)
    shader = new Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
    
    ParseTextures(jsonData);
    ParseUniforms(jsonData);
    file.close();
}


void Material::SetFloat(const std::string& name, float value) {
    if (!shader->doesUniformExist(name)) {
        SPDLOG_ERROR("Uniform: " + name + " not found");
        return;
    }

    RemoveUniform(name);

    auto newUniform = std::make_unique<FloatUniform>();
    newUniform->name = name;
    newUniform->value = value;
    uniforms.push_back(std::move(newUniform));
}

void Material::SetInt(const std::string& name, int value) {
    if (!shader->doesUniformExist(name)) {
        SPDLOG_ERROR("Uniform: " + name + " not found");
        return;
    }

    RemoveUniform(name);

    auto newUniform = std::make_unique<IntUniform>();
    newUniform->name = name;
    newUniform->value = value;
    uniforms.push_back(std::move(newUniform));
}

void Material::SetVec4(const std::string& name, glm::vec4 value) {
    if (!shader->doesUniformExist(name)) {
        SPDLOG_ERROR("Uniform: " + name + " not found");
        return;
    }

    RemoveUniform(name);

    auto newUniform = std::make_unique<Vec4Uniform>();
    newUniform->name = name;
    newUniform->value = value;
    uniforms.push_back(std::move(newUniform));
}




void Material::SetTexture(const std::string& name,Texture* texture)
 {
        if (!shader->doesUniformExist(name)) {
            SPDLOG_ERROR("Uniform: " + name + " not found");
            return;
        }

        RemoveUniform(name);
        auto newUniform = std::make_unique<TextureUniform>();
        newUniform->name = name;
        newUniform->textureAsset = texture;
        uniforms.push_back(std::move(newUniform));
 }

void Material::RemoveUniform(const std::string& name) {
    uniforms.erase(std::remove_if(uniforms.begin(), uniforms.end(),
        [name](const std::unique_ptr<Uniform>& uniform) { return uniform->name == name; }),
        uniforms.end());
}

void Material::ParseTextures(const json& materialJson) {
    for (const auto& jsonTexture : materialJson["textures"]) {
        std::string path = jsonTexture["path"].get<std::string>();

        auto newUniform = std::make_unique<TextureUniform>();
        newUniform->name = jsonTexture["name"];
        //memory leak
        newUniform->textureAsset = new Texture(path, jsonTexture["name"]);
        newUniform->index = jsonTexture["index"].get<int>();
        uniforms.push_back(std::move(newUniform));
    }
}

void Material::ParseUniforms(const json& materialJson)
{
    for (const auto& jsonUniform : materialJson["uniforms"]) {
        std::string type = jsonUniform["type"].get<std::string>();

        if (type == "float") {
            SetFloat(jsonUniform["name"], jsonUniform["value"]);
        }
        else if (type == "int") {
            SetInt(jsonUniform["name"], jsonUniform["value"]);
        }
        else if (type == "vec4") {
            SetVec4(jsonUniform["name"], {
                                                 jsonUniform["value"][0],
                                                 jsonUniform["value"][1],
                                                 jsonUniform["value"][2],
                                                 jsonUniform["value"][3],
                });
        }
        else {
            SPDLOG_ERROR("MATERIAL: Unknown uniform type {}", type);
        }
    }
}

Shader* Material::getShader()
{
    return shader;
}


void Material::bindMaterial()
{
    shader->use();
    for (const auto& uniform : uniforms) {
        uniform->ApplyValue(shader);
    }
}

void Material::unbindMaterial()
{
    glUseProgram(0);
}

void Material::Vec4Uniform::ApplyValue(Shader* shader)
{
    shader->setVec4(name, value);
}

std::unique_ptr<Material::Uniform> Material::Vec4Uniform::Clone()
{
    auto clone = std::make_unique<Material::Vec4Uniform>();
    clone->name = name;
    clone->value = value;

    return std::move(clone);
}
void Material::TextureUniform::ApplyValue(Shader* shader)
{
    glActiveTexture(GL_TEXTURE0 + index);
    textureAsset->bind();
    shader->setInt(name, index);
}

std::unique_ptr<Material::Uniform> Material::TextureUniform::Clone() {
    auto clone = std::make_unique<Material::TextureUniform>();
    clone->name = name;
    clone->textureAsset = textureAsset;
    clone->index = index;

    return std::move(clone);
}

void Material::FloatUniform::ApplyValue(Shader* shader)
{
    shader->setFloat(name, value);
}

std::unique_ptr<Material::Uniform> Material::FloatUniform::Clone()
{
    auto clone = std::make_unique<Material::FloatUniform>();
    clone->name = name;
    clone->value = value;

    return std::move(clone);
}

void Material::IntUniform::ApplyValue(Shader* shader)
{
    shader->setInt(name, value);
}

std::unique_ptr<Material::Uniform> Material::IntUniform::Clone()
{
    auto clone = std::make_unique<Material::IntUniform>();
    clone->name = name;
    clone->value = value;

    return std::move(clone);
}
