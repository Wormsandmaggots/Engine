#include "Renderer/MaterialAsset.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


MaterialAsset::MaterialAsset(const std::string& path) : materialPath(path)
{
    // Read and parse JSON file
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open JSON file: " + path);
    }

    json jsonData;
    file >> jsonData;

    // Extract shader paths
    std::string vertexShaderPath = jsonData["vertexShader"];
    std::string fragmentShaderPath = jsonData["fragmentShader"];

    // Initialize shader (implementation of Shader class assumed)
    shader = new Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
    


}





