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
#include "Animation/Bone.h"
#include "assimp_glm_helpers.h"

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
        rootBone = new Bone();
        ReadHierarchyData(scene->mRootNode, rootBone, NULL);
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
        destBone->setTransform(AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation));
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

    void CalculateBoneTransform(glm::mat4 parentTransform, Bone* bone){

        std::string nodeName = bone->getName();
        glm::mat4 nodeTransform = bone->getTransform();
        glm::mat4 globalTransformation = parentTransform * nodeTransform;
        if(m_BoneInfoMap[nodeName].id == 14){
            glm::vec3 translation = glm::vec3(globalTransformation[3]);
            std::cout << "Translation: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << std::endl;
        }
        if(m_BoneInfoMap[nodeName].id == 12){
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            globalTransformation *= rotation; // Multiply the rotation with the existing transformation
        }
        if (m_BoneInfoMap.find(nodeName) != m_BoneInfoMap.end())
        {
            int index = m_BoneInfoMap[nodeName].id;
            glm::mat4 offset = m_BoneInfoMap[nodeName].offset;
            m_FinalBoneMatrices[index] = globalTransformation * offset;
        }
        for (int i = 0; i < bone->getChildrenCount(); i++)
            CalculateBoneTransform(globalTransformation, bone->getChildren()[i]);
    }

    void update(){
        CalculateBoneTransform(glm::mat4(1.0f), rootBone);
    }

    std::vector<glm::mat4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

    void ik(std::string limb, glm::vec3 desiredPos){
        Bone* limbBone;
        if(bones.find(limb)!=bones.end()){ //searching for limb in our bones
            limbBone = bones[limb];
        }
        Bone* secondToLast = limbBone->getParent();
        glm::vec3 e_i = glm::vec3(desiredPos - AssimpGLMHelpers::GetTransformFromMatrix(secondToLast->getParent()->getTransform()));
        glm::vec3 t_i = glm::vec3(AssimpGLMHelpers::GetTransformFromMatrix(limbBone->getTransform()) - AssimpGLMHelpers::GetTransformFromMatrix(secondToLast->getParent()->getTransform()));
        e_i = glm::normalize(e_i);
        t_i = glm::normalize(t_i);
        const float angle = glm::acos(glm::dot(e_i,t_i)); //angle between two vectors
        if(angle >0.001f){
            glm::vec3 axis = glm::cross(e_i,t_i); //rotation axis
            axis = glm::normalize(axis);

            glm::quat rot(angle, axis.x, axis.y, axis.z); //to check if was initialized correctly

        }

    }
private:
    Bone* rootBone;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
    std::vector<glm::mat4> m_FinalBoneMatrices;
    std::map<std::string,Bone*> bones;
};
#endif //ENGINE_RIGPREP_H
