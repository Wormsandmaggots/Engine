#ifndef ENGINE_RIGPREP_H
#define ENGINE_RIGPREP_H
//!class that is a merge between Animation and Animator from LearnOpenGl
//!prepares hierarchy of bones and converts vectors from local to global space based on this hierarchy
//!as a result we get a mat4 ready to be passed to shader
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "Model.h"
#include "assimp_glm_helpers.h"

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};
class RigPrep{
public:
    RigPrep(Model* model){
        //const aiScene* scene = model->getScene();
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(model->getPath(),
                                                 aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
                                                 aiProcess_CalcTangentSpace);
        m_BoneInfoMap = model->getMap();
        assert(scene && scene->mRootNode);
        aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
        globalTransformation = globalTransformation.Inverse();
        m_FinalBoneMatrices.resize(m_BoneInfoMap.size());
        ReadHierarchyData(m_RootNode, scene->mRootNode);
    }
    ~RigPrep()= default;
    inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
    inline const std::map<std::string,BoneInfo>& GetBoneIDMap()
    {
        return m_BoneInfoMap;
    }
    void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
    {
        assert(src);

        dest.name = src->mName.data;
        dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
        dest.childrenCount = src->mNumChildren;

        for (int i = 0; i < src->mNumChildren; i++)
        {
            AssimpNodeData newData;
            ReadHierarchyData(newData, src->mChildren[i]);
            dest.children.push_back(newData);
        }
    }

    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform){
        std::string nodeName = node->name;
        glm::mat4 nodeTransform = node->transformation;
        glm::mat4 globalTransformation = parentTransform * nodeTransform;
        if (m_BoneInfoMap.find(nodeName) != m_BoneInfoMap.end())
        {
            int index = m_BoneInfoMap[nodeName].id;
            glm::mat4 offset = m_BoneInfoMap[nodeName].offset;
            m_FinalBoneMatrices[index] = globalTransformation * offset;
        }

        for (int i = 0; i < node->childrenCount; i++)
            CalculateBoneTransform(&node->children[i], globalTransformation);
    }

    void update(){
        CalculateBoneTransform(&m_RootNode, glm::mat4(1.0f));
    }
    std::vector<glm::mat4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }
private:
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
    std::vector<glm::mat4> m_FinalBoneMatrices;
};
#endif //ENGINE_RIGPREP_H
