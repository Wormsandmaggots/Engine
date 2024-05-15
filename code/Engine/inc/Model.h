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
/*
    Mesh generateAABBBoundingBoxMesh(const CPM_GLM_AABB_NS::AABB& aabb) {
        glm::vec3 min = aabb.getMin();
        glm::vec3 max = aabb.getMax();

        // Wierzchołki dla AABB
        std::vector<Vertex> vertices = {
                {{min.x, min.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                {{max.x, min.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                {{max.x, max.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                {{min.x, max.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                {{min.x, min.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                {{max.x, min.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                {{max.x, max.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                {{min.x, max.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}
        };

        // Indeksy dla AABB
        std::vector<unsigned int> indices = {
                0, 1, 1, 2, 2, 3, 3, 0, // dolna ściana
                4, 5, 5, 6, 6, 7, 7, 4, // górna ściana
                0, 4, 1, 5, 2, 6, 3, 7  // krawędzie boczne
        };

        // Tworzymy siatkę bez tekstur
        return Mesh(vertices, indices);
    }
    */
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