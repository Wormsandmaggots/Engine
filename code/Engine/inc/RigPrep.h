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
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

class RigPrep{
public:
    RigPrep(Model* model){
        //const aiScene* scene = model->getScene();
        rigsModel = model;
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

    void CalculateBoneTransform(glm::mat4 parentTransform, Bone* bone, int offset){

        std::string nodeName = bone->getName();
        glm::mat4 localSpaceTransform = bone->getLocalTransform(); //local (bone) space
        /*
        ///DEBUG SPACE
        if(m_BoneInfoMap[nodeName].id == 14){
            glm::vec3 translation = glm::vec3(localSpaceTransform[3]);
            std::cout << "Local space: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << std::endl;

        }*/
        glm::mat4 modelSpaceTransformation = parentTransform * localSpaceTransform; //model space that shader requires
        bone->setModelTransform(modelSpaceTransformation);
        /*
        if(m_BoneInfoMap[nodeName].id == 14){
            glm::vec3 translation = glm::vec3(modelSpaceTransformation[3]);
            std::cout << "Model space: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << std::endl;
            glm::vec3 epos =  computeEndEffectorPosition(bone);
            std::cout << "Effector pos: " << epos.x << " " << epos.y << " " << epos.z <<std::endl;
        }*/


        ///HAND TRANSLATION
        if(m_BoneInfoMap[nodeName].id == 13){
            //glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            //modelSpaceTransformation *= rotation; // Multiply the rotation with the existing transformation
            modelSpaceTransformation = glm::translate(modelSpaceTransformation, glm::vec3(0.0f, 0.0f, offset));
        }

        //DEBUG SPACE
        if(m_BoneInfoMap[nodeName].id == 14){
            glm::vec3 translation = glm::vec3(modelSpaceTransformation[3]);
            std::cout << "Model space after translation: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << std::endl;
        }/*
        glm::mat4 globalSpaceTransformation = modelSpaceTransformation * rigsModel->getLocalTransform()->getLocalMatrix(); //model space that shader requires
        if(m_BoneInfoMap[nodeName].id == 14){
            glm::vec3 translation = glm::vec3(globalSpaceTransformation[3]);
            std::cout << "Global space: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << std::endl;
        }*/


        if (m_BoneInfoMap.find(nodeName) != m_BoneInfoMap.end())
        {
            int index = m_BoneInfoMap[nodeName].id;
            glm::mat4 offset = m_BoneInfoMap[nodeName].offset;
            m_FinalBoneMatrices[index] = modelSpaceTransformation * offset; //vertices to model space
            /*if(index == 14){
                glm::vec3 translation = (modelSpaceTransformation * offset)[3];
                std::cout << "no idea: (" << translation.x << ", " << translation.y << ", " << translation.z<< ")" << std::endl;

            }*/
        }
        for (int i = 0; i < bone->getChildrenCount(); i++)
            CalculateBoneTransform(modelSpaceTransformation, bone->getChildren()[i], offset);
    }

    void update(int offset){
        CalculateBoneTransform(glm::mat4(1.0f), rootBone, offset);
        ik("mixamorig:RightHand");
    }

    std::vector<glm::mat4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

    void ik(std::string limb){
        glm::vec3 target = glm::vec3(-124.35f, 688.36f, -23.87f);
        Bone* limbBone;
        if(bones.find(limb)!=bones.end()){ //searching for limb in our bones
            limbBone = bones[limb];
        }
        glm::vec3 endEffector = limbBone ->getModelPosition();
        for(int j = 0; j <3; j++){ //petla by zwiekszyc dokladnosc wyniku
            Bone* secondToLast = limbBone->getParent();
            for (int i =0; i <3; i++) { //petla by przejsc 3 poprzednie kosci
                glm::vec3 e_i = glm::vec3(endEffector - secondToLast->getModelPosition());
                glm::vec3 t_i = glm::vec3(target - secondToLast->getModelPosition());
                e_i = glm::normalize(e_i);
                t_i = glm::normalize(t_i);
                float angle = glm::acos(glm::dot(e_i,t_i)); //angle between two vectors
                if(angle >0.001f){
                    glm::vec3 axis = glm::cross(e_i,t_i); //rotation axis
                    axis = glm::normalize(axis);
                    glm::quat quat = glm::angleAxis(angle, axis);
                    glm::quat inv_quat = glm::inverse(glm::quat(secondToLast->getModelTransform()));
                    glm::mat4 invRotationMatrix = glm::toMat4(inv_quat);
                    glm::mat4  mat = glm::rotate(secondToLast->getModelTransform(), angle, axis); ///JAK COS JEST ZLE TO PEWNIE TU
                    secondToLast->setModelTransform(mat);
                    glm::mat4 newTransform = invRotationMatrix * secondToLast->getModelTransform();
                    secondToLast->setLocalTransform(newTransform);
                }
                secondToLast = secondToLast->getParent();
                endEffector = limbBone -> getModelPosition();
            }
        }


    }

    glm::vec3 computeEndEffectorPosition(Bone* endEffectorBone) {
        glm::mat4 accumulatedTransform = glm::mat4(1.0f);
        glm::vec4 worldPosition = glm::vec4(1.0f);
        //from the end-effector bone to the root
        while (endEffectorBone != nullptr) {
            accumulatedTransform = endEffectorBone->getLocalTransform() * accumulatedTransform;

            // Transform the local position of the end-effector bone to world space
            glm::vec4 localPosition = glm::vec4(endEffectorBone->getLocalPosition(), 1.0f); // Assuming getLocalPosition() returns local position
            worldPosition = accumulatedTransform * localPosition;

            // Move to the parent bone
            endEffectorBone = endEffectorBone->getParent();
        }

        return glm::vec3(worldPosition);
    }
private:
    Bone* rootBone;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
    std::vector<glm::mat4> m_FinalBoneMatrices;
    std::map<std::string,Bone*> bones;
    Model* rigsModel;
};
#endif //ENGINE_RIGPREP_H
