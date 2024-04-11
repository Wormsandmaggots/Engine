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

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model: public Component, public Renderable
{
public:
    vector<Texture> textures_loaded;
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;
    Model(const string &path, bool gamma=false);
    void Draw(Shader* shader) override;

    ~Model() override = default;

    void awake() override;

    void start() override;

    void update() override;

    void onDestroy() override;

    string serialize() override;

    void setParent(Entity *entity) override;

    void setTransform(Transform2 *transform2) override;

private:
    void loadModel(string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};



#endif