#ifndef OPENGLGP_MESH_H
#define OPENGLGP_MESH_H
#define MAX_BONE_INFLUENCE 4

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <vector>
#include <tracy/Tracy.hpp>
#include "Shader.h"
#include "ECS/Component.h"
#include "Renderer/Texture.h"
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct SVertex{
    glm::vec3 Position;
    glm::vec2 TexCoords;
};


class Mesh
{
private:
    unsigned int VBO, EBO, instanceBuffer;
    virtual void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        // ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        // weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        glBindVertexArray(0);
    }
    virtual void setupMeshInstanced()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        // create instance buffer
        glGenBuffers(1, &instanceBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
        glBufferData(GL_ARRAY_BUFFER, instances * sizeof(glm::mat4), &instancedMatrix[0], GL_STATIC_DRAW);

        // set up instance attributes
        for (unsigned int i = 0; i < 4; i++)
        {
            glEnableVertexAttribArray(5 + i);
            glVertexAttribPointer(5 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
            glVertexAttribDivisor(5 + i, 1);
        }

        glBindVertexArray(0);
    }

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    std::vector<SVertex>* sphereVertices;

    std::vector<glm::mat4>* instancedMatrix;

    unsigned int instances;

    bool isGenerated = false;

    unsigned int VAO;

    /*  Functions  */
    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, unsigned int numberOfInstances = 1, std::vector<glm::mat4>* instancedMatrix = nullptr)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        instances = numberOfInstances;
        this->instancedMatrix = instancedMatrix;
        if (numberOfInstances > 1) {
            setupMeshInstanced();
        }
        else {
            setupMesh();
        }
    }

    // render the mesh
    void Draw(Shader* shader,const std::string& path, bool instanced = false, int amount = 0)
    {
        std::string str = "";
        if(textures.size()>0){
            str = textures[0].texturePath;
        }
        else{
            str =path;
        }
        ZoneTransientN(Draw,str.c_str(), true);
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int metalnessNr = 1;
        unsigned int normalNr = 1;
        unsigned int ambientNr = 1;
        unsigned int roughnessNr = 1;
        unsigned int displacementNr = 1;
        unsigned int emissiveNr = 1;
        bool useEmissive = false;
        std::map<TextureType, int> textureNumberMap;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = textures[i].textureName;
            number = std::to_string(1 + textureNumberMap[textures[i].textureType]++);

//            if(textures[i].textureType == TextureType::EMISSIVE)
//            {
//                useEmissive = true;
//            }
//            if (type == TextureType::DIFFUSE)
//                number = std::to_string(diffuseNr++);
//            else if (type == TextureType::SPECULAR)
//                number = std::to_string(specularNr++);
//            else if (type == TextureType::METALLIC)
//                number = std::to_string(metalnessNr++);
//            else if (type == TextureType::NORMAL)
//                number = std::to_string(normalNr++);
//            else if (type == TextureType::AMBIENT)
//                number = std::to_string(ambientNr++);
//            else if (type == TextureType::ROUGHNESS)
//                number = std::to_string(roughnessNr++);
//            else if (type == TextureType::DISPLACEMENT)
//                number = std::to_string(displacementNr++);
//            else if (type == TextureType::EMISSIVE)
//                number = std::to_string(emissiveNr++);
//            if (name == "texture_diffuse")
//                number = std::to_string(diffuseNr++);
//            else if (name == "texture_specular")
//                number = std::to_string(specularNr++);
//            else if (name == "texture_metalic")
//                number = std::to_string(metalnessNr++);
//            else if (name == "texture_normal")
//                number = std::to_string(normalNr++);
//            else if (name == "texture_ambient")
//                number = std::to_string(ambientNr++);
//            else if (name == "texture_roughness")
//                number = std::to_string(roughnessNr++);
//            else if (name == "texture_displacement")
//                number = std::to_string(displacementNr++);
//            else if (name == "texture_emissive")
//                number = std::to_string(emissiveNr++);
            //now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
            textures[i].bind();
        }

//        if(useEmissive)
//        {
//            shader->setBool("useEmissive", true);
//        }
//        else
//        {
//            shader->setBool("useEmissive", false);
//        }

        // draw mesh
        if(!isGenerated && !instanced)
        {
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        } else if(!instanced){
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, this->sphereVertices->size());
            glBindVertexArray(0);
        } else{
            glBindVertexArray(VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }


        for (GLuint i = 0; i < this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

    }

    void drawInstanced(Shader* shader, unsigned int instanceCount, glm::mat4 translationMatrix) 
    {
    
    }
};




#endif //OPENGLGP_MESH_H