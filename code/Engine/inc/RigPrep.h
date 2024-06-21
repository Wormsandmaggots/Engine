#ifndef ENGINE_RIGPREP_H
#define ENGINE_RIGPREP_H
//!prepares hierarchy of bones and converts vectors from local to model space based on this hierarchy
//!as a result we get a mat4 ready to be passed to shader
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "Model.h"
#include "Animation/Bone.h"
#include "assimp_glm_helpers.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include "Renderer/InstancedRobots.h"

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
        m_FinalBoneMatrices.resize(m_BoneInfoMap.size());
        rootBone = new Bone();
        ReadHierarchyData(scene->mRootNode, rootBone, NULL);
        CalculateBoneTransform(glm::mat4(1.0f), rootBone, 1.0f);
    }

    RigPrep(InstancedRobots* instanced)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(instanced->path,
                                                 aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
                                                 aiProcess_CalcTangentSpace);
        m_BoneInfoMap = instanced->getMap();
        assert(scene && scene->mRootNode);
        m_FinalBoneMatrices.resize(m_BoneInfoMap.size());
        rootBone = new Bone();
        ReadHierarchyData(scene->mRootNode, rootBone, NULL);
        CalculateBoneTransform(glm::mat4(1.0f), rootBone, 1.0f);
    }

    ~RigPrep()= default;
    inline const Bone* GetRootBone() { return rootBone; }
    inline const std::map<std::string,BoneInfo>& GetBoneIDMap()
    {
        return m_BoneInfoMap;
    }
    void ReadHierarchyData( const aiNode* src, Bone* destBone, Bone* parentBone)
    {
        assert(src);

        destBone->setName(src->mName.data);
        destBone->setLocalTransform(AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation));
        destBone->setChildrenCount(src->mNumChildren);
        destBone->setParent(parentBone);

        for (int i = 0; i < src->mNumChildren; i++)
        {
            Bone* newBone = new Bone();
            destBone->getChildren().push_back(newBone);
            ReadHierarchyData(src->mChildren[i],newBone,destBone);

        }
        bones.insert({destBone->getName(),destBone});
    }

    void SwapHierarchyData(){
    }

    void CalculateBoneTransform(glm::mat4 parentTransform, Bone* bone, int offset){

        std::string nodeName = bone->getName();
        glm::mat4 localSpaceTransform = bone->getLocalTransform(); //local (bone) space

        glm::mat4 modelSpaceTransformation = parentTransform * localSpaceTransform; //model space that shader requires
        bone->setModelTransform(modelSpaceTransformation);

        if (m_BoneInfoMap.find(nodeName) != m_BoneInfoMap.end())
        {
            int index = m_BoneInfoMap[nodeName].id;
            glm::mat4 offset = m_BoneInfoMap[nodeName].offset;
            m_FinalBoneMatrices[index] = modelSpaceTransformation * offset; //vertices to model space
        }
        for (int i = 0; i < bone->getChildrenCount(); i++)
            CalculateBoneTransform(modelSpaceTransformation, bone->getChildren()[i], offset);
    }

    void calculateVertices(glm::mat4 parentTransform, Bone* bone){
        std::string nodeName = bone->getName();
        if (m_BoneInfoMap.find(nodeName) != m_BoneInfoMap.end())
        {
            int index = m_BoneInfoMap[nodeName].id;
            glm::mat4 offset = m_BoneInfoMap[nodeName].offset;
            m_FinalBoneMatrices[index] = bone->getModelTransform() * offset; //vertices to model space
        }
        for (int i = 0; i < bone->getChildrenCount(); i++)
            calculateVertices(bone->getModelTransform(), bone->getChildren()[i]);
    }

    void update(){
        calculateVertices(glm::mat4(1.0f), rootBone);
    }

    void swapBones(std::vector<glm::mat4> matrix){
        m_FinalBoneMatrices = matrix;
        bones.clear();

    }

    std::vector<glm::mat4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

    std::map<std::string,Bone*> getBones(){
        return bones;
    }

    void setBones(std::map<std::string,Bone*> _bones){
        bones = _bones;
        update();
    }


    Bone* getBone(std::string name){
        return bones[name];
    }
private:
    Bone* rootBone;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
    std::vector<glm::mat4> m_FinalBoneMatrices;
    std::map<std::string,Bone*> bones;
};
#endif //ENGINE_RIGPREP_H
