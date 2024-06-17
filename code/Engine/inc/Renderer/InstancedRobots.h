//
// Created by Radek on 16.06.2024.
//

#ifndef ENGINE_INSTANCEDROBOTS_H
#define ENGINE_INSTANCEDROBOTS_H
#include "ECS/Component.h"
#include "Texture.h"
#include "Mesh.h"
#include <assimp/scene.h>
#include <map>
#include "Renderer/Renderable.h"
#include "Model.h"

using namespace std;

class InstancedRobots : public Component, public Renderable {
public:
    vector<Texture> textures_loaded;
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;
    bool instanced;
    glm::ivec2 amount;
    Shader* modelShader;
    std::string path;



    InstancedRobots(const string &path, glm::ivec2 gridSize, Shader *shader,
                    glm::vec3 startPoint = glm::vec3(0),
                    glm::vec3 offset = glm::vec3(2.f),
                    glm::vec3 scale = glm::vec3(1.f)){
        instanced = true;
        this->amount = gridSize;
        modelShader = shader;
        this->path = path;
        directory = path.substr(0, path.find_last_of('/'));

        translations = new glm::mat4[amount.x * amount.y]{};

        loadModel(path);
        setupTranslations(startPoint, offset, scale);
        setupMeshForInstancing();
    }

    void setupMeshForInstancing()
    {
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, amount.x * amount.y * sizeof(glm::mat4), &translations[0], GL_STATIC_DRAW);

        // set transformation matrices as an instance vertex attribute (with divisor 1)
        // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
        // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
        // -----------------------------------------------------------------------------------------------------------------------------------
        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            unsigned int VAO = meshes[i].VAO;
            glBindVertexArray(VAO);
            // set attribute pointers for matrix (4 times vec4)
            glEnableVertexAttribArray(7);
            glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
            glEnableVertexAttribArray(8);
            glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
            glEnableVertexAttribArray(9);
            glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(10);
            glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

            glVertexAttribDivisor(7, 1);
            glVertexAttribDivisor(8, 1);
            glVertexAttribDivisor(9, 1);
            glVertexAttribDivisor(10, 1);

            glBindVertexArray(0);
        }
    }

    void SetVertexBoneDataToDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
    void setupTranslations(glm::vec3 startPoint, glm::vec3 offset, glm::vec3 scale);

    void Draw(Shader* shader) override;
    void update() override;
    void awake() override {};
    Shader * getShader() override {return modelShader;};
    void start() override {};
    void onDestroy() override {};
    Transform2 * getTransform() override {return parentTransform;};
    int& GetBoneCount() { return m_BoneCounter;}

    std::map<string, BoneInfo> getMap() {
        return m_BoneInfoMap;
    }

private:
    void loadModel(string const &path);
    const aiScene* modelScene;
    void processNode(aiNode* node, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::map<string, BoneInfo> m_BoneInfoMap;
    int m_BoneCounter = 0;
    glm::mat4* translations;
};


#endif //ENGINE_INSTANCEDROBOTS_H
