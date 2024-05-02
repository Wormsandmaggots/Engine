#include "Model.h"
#include "imgui.h"
#include "Editor/Panels/FileDialog.h"
#include <GLFW/glfw3.h>
#include "assimp_glm_helpers.h"
void Model::awake() {}

void Model::start() {}

void Model::update() { 
    Renderer::Render(this); 
}

void Model::onDestroy() {}

Shader* Model::getShader()
{
    return modelShader;
}

Transform2* Model::getTransform()
{
    
    return parentTransform;
}


Model::Model(const string& path, Shader* shader, bool gamma)
{
    modelShader = shader;
    this->path = path;
    directory = path.substr(0, path.find_last_of('/'));

    loadModel(path);
}

Model::Model(const string& path, MaterialAsset* material)
{
    this->path = path;
    modelShader = material->getShader();
    this->material = material;

    directory = path.substr(0, path.find_last_of('/'));

    loadModel(path);
}

Model::Model(const Model& another) {
    directory = path.substr(0, path.find_last_of('/'));
    this->path = another.path;
    this->modelShader = another.modelShader;

    loadModel(path);
}

Model::Model() {
    path = "res/content/models/sphere/untitled.obj";
    modelShader = nullptr;
    //PATH JEST DO SPHERE a nie do obiektu z którego powinno przyjmować texture
    directory = path.substr(0, path.find_last_of('/'));

    loadModel(path);
}

void Model::Draw(Shader *shader) {

    if(material!=nullptr)
        material->bindMaterial();

    for (auto &meshe: meshes)
        meshe.Draw(*shader);

    if (material != nullptr)
    material->unbindMaterial();

}

void Model::loadModel(string const &path) {
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

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode *node, const aiScene *scene) {
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

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
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
vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
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

void Model::convertToYaml(YAML::Emitter &emitter) {
    emitter << YAML::Key << "Type" << YAML::Value << "Model";

    emitter << YAML::Key << "Path" << YAML::Value << path;
}

void Model::setPath(std::string path) {
    this->path = path;
}

void Model::drawEditor() {
    ImGui::Text("Model");

    if(ImGui::Button(path.c_str()))
    {
        std::string newPath = FileDialog::openFile("");

        if(!newPath.empty())
        {
            setPath(newPath);
            textures_loaded.clear();
            meshes.clear();
            loadModel(newPath);
        }
    }

    ImGui::NewLine();
}

auto& Model::GetBoneInfoMap() {return m_BoneInfoMap;}
int& Model::GetBoneCount() { return m_BoneCounter;}
void Model::SetVertexBoneDataToDefault(Vertex& vertex)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.m_BoneIDs[i] = -1;
        vertex.m_Weights[i] = 0.0f;
    }
}
void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
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

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
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

const aiScene* Model::getScene() {
    return modelScene;
}

const std::map<string, BoneInfo> Model::getMap() {
    return m_BoneInfoMap;
}

std::string Model::getPath() {
    return path;
}
