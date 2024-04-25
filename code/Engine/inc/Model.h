#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "ECS/Component.h"
#include "Renderer/Renderable.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"
#include "Renderer/MaterialAsset.h"

using namespace std;


class Model: public Component, public Renderable
{
public:
    bool isVisible = false;
    vector<Texture> textures_loaded;
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;
    Model();
    Model(const Model&);
    Model(const string &path,Shader* shader=nullptr, bool gamma=false);
    Model(const string& path, MaterialAsset* material);
    void Draw(Shader* shader) override;

    ~Model() override = default;

    void awake() override;

    void start() override;

    void update() override ;

    void onDestroy() override;

    Shader* getShader()override;
    Transform2* getTransform()override;

    void convertToYaml(YAML::Emitter &) override;
    void drawEditor() override;
    void setPath(std::string);
private:
    Shader* modelShader;
    std::string path;
    MaterialAsset * material=nullptr;
    void loadModel(string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};



#endif