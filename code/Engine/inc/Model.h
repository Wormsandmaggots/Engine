#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp   >
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
#include "Core/AssetManager/Asset.h"

using namespace std;

struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;

    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;

};
class Model: public Component, public Renderable
{
public:
    vector<Texture> textures_loaded;
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;
    bool instanced;
    bool amount;

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
    void setPath(std::string);
    const aiScene* getScene();
    std::string getPath();
    std::map<string, BoneInfo> getMap();
    auto& GetBoneInfoMap();
    int& GetBoneCount();
    void SetVertexBoneDataToDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
    MaterialAsset* getMaterial();
    bool enable = true;
private:
    Shader* modelShader;
    std::string path;
    MaterialAsset * material=nullptr;
    void loadModel(string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::map<string, BoneInfo> m_BoneInfoMap;
    int m_BoneCounter = 0;
    const aiScene* modelScene;
};



#endif