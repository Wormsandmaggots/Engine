//
// Created by Radek on 16.06.2024.
//

#include "Renderer/InstancedRobots.h"
#include "assimp_glm_helpers.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Renderer/Renderer.h"

void InstancedRobots::loadModel(string const &path) {
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,
                                             aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
                                             aiProcess_CalcTangentSpace);
    modelScene = new aiScene(*scene);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    // retrieve the directory path of the filepath
    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void InstancedRobots::processNode(aiNode *node, const aiScene *scene) {
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }

}

Mesh InstancedRobots::processMesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        SetVertexBoneDataToDefault(vertex);

        vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
        vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);

        if (mesh->mTextureCoords[0]) // czy siatka zawiera współrzędne tekstury?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        vector<Texture> specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        vector<Texture> metalnessMaps = loadMaterialTextures(material,
                                                             aiTextureType_METALNESS, "texture_metalic");
        textures.insert(textures.end(), metalnessMaps.begin(), metalnessMaps.end());
        vector<Texture> ambientMaps = loadMaterialTextures(material,
                                                           aiTextureType_AMBIENT, "texture_ambient");
        textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
        vector<Texture> roughnessMaps = loadMaterialTextures(material, aiTextureType_SHININESS, "texture_roughness");
        textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
        vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        vector<Texture> displacementMaps = loadMaterialTextures(material,
                                                                aiTextureType_DISPLACEMENT, "texture_displacement");
        textures.insert(textures.end(), displacementMaps.begin(), displacementMaps.end());
    }
    ExtractBoneWeightForVertices(vertices,mesh,scene);
    return Mesh(vertices, indices, textures);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
vector<Texture> InstancedRobots::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].texturePath.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip) {   // if texture hasn't been loaded already, load it
            Texture texture(directory + "/" + str.C_Str(), typeName);
            textures.push_back(texture);
            textures_loaded.push_back(
                    texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}

void InstancedRobots::SetVertexBoneDataToDefault(Vertex& vertex)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.m_BoneIDs[i] = -1;
        vertex.m_Weights[i] = 0.0f;
    }
}
void InstancedRobots::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.m_BoneIDs[i] < 0)
        {
            vertex.m_Weights[i] = weight;
            vertex.m_BoneIDs[i] = boneID;
            break;
        }
    }
}

void InstancedRobots::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = m_BoneCounter;
            newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(
                    mesh->mBones[boneIndex]->mOffsetMatrix);
            m_BoneInfoMap[boneName] = newBoneInfo;
            boneID = m_BoneCounter;
            m_BoneCounter++;
        }
        else
        {
            boneID = m_BoneInfoMap[boneName].id;
        }
        assert(boneID != -1);
        auto weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            assert(vertexId <= vertices.size());
            SetVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}

void InstancedRobots::setupTranslations(glm::vec3 startPoint, glm::vec3 offset, glm::vec3 scale) {
    translations = new glm::mat4[amount.x * amount.y];

    for(int i = 0; i < amount.x; i++)
    {
        for(int j = 0; j < amount.y; j++)
        {
            glm::mat4 m = glm::mat4(1.f);

            m = glm::translate(m, startPoint);
            m = glm::scale(m, scale);
            m = glm::translate(m, glm::vec3(offset.x * i, offset.y * i, offset.z * j));

            translations[i * amount.x + j] = glm::mat4(m);
        }

    }
}

void InstancedRobots::Draw(Shader *shader) {
    //m->DrawInstanced(shader, amount);

    //shader->use();

    for (Mesh& mesh : meshes) {
        mesh.Draw(shader, path, instanced, amount.x * amount.y);
        //mesh.Draw(*shader, path);
    }
}

void InstancedRobots::update() {
    Renderer::Render(this);
}
