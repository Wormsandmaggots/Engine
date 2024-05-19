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
        m_FinalBoneMatrices.resize(m_BoneInfoMap.size());
        rootBone = new Bone();
        ReadHierarchyData(scene->mRootNode, rootBone, NULL);
        CalculateBoneTransform(glm::mat4(1.0f), rootBone, 1.0f);
        hingeAxis = glm::vec3(0.0f,1.0f,0.0f);
        initTarget = glm::vec3(-40, 800, -23);
        prevOffset = -1;
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
    void rotateBone(int offset){
        float angle = offset;
        std::cout << angle <<std::endl;
        glm::vec3 axis = glm::vec3(0.0f,1.0f,0.0f);
        Bone* forearm = bones["mixamorig:RightForeArm"];

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
        glm::mat4 newModelTransform = rotationMatrix * forearm->getModelTransform();
        forearm->setModelTransform(newModelTransform);

        //glm::mat4  mat = glm::rotate(forearm->getModelTransform(), glm::radians(angle), axis);
        //forearm->setModelTransform(mat);


        glm::mat4 parentModelMatrix = forearm->getParent()->getModelTransform();
        glm::mat3 parentRotationPart = glm::mat3(parentModelMatrix);
        glm::mat3 invParentRotationPart = glm::inverse(parentRotationPart); //odwrotnoscc macierzy oblicza sie prawidlowo

        //dopasowanie localSpace na podstawie zmian wprowadzonych w modelSpace
        glm::mat4 newLocalMatrix = glm::mat4(invParentRotationPart) * forearm->getModelTransform(); //oblicza sie prawidlowo
        forearm->setLocalTransform(newLocalMatrix);

        updateChildren(forearm);
    }

    void update(int offset){
        //if(offset > prevOffset+10 || prevOffset == -1){
            ik("mixamorig:RightHand",offset);
            //prevOffset = offset;
        //}

        //rotateBone(offset);
        calculateVertices(glm::mat4(1.0f), rootBone);
    }



    std::vector<glm::mat4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

///IK Z MACIERZAMI
    void ik(std::string limb, int offset){
        Bone* limbBone;
        if(bones.find(limb)!=bones.end()){ //searching for limb in our bones
            limbBone = bones[limb];
        }
        //glm::vec3 target = limbBone->getModelPosition();

        //glm::vec3 target = initTarget + vec3(offset, 0, 0);
        glm::vec3 target = initTarget;
        //std::cout << target.x << " " << target.y << " " << target.z << std::endl;
        glm::vec3 endEffector = limbBone ->getModelPosition();
        std::cout << "NOWA KLATKA"<<std::endl;
        for(int j = 0; j <1; j++){ //petla by zwiekszyc dokladnosc wyniku
            Bone* secondToLast = limbBone->getParent(); //przypisanie przedramienia jako kowsci ktora manewrujemy jako pierwsza
            for (int i =0; i <2; i++) { //petla by przejsc 3 poprzednie kosci
                glm::vec3 e_i = glm::vec3(endEffector - secondToLast->getModelPosition());
                glm::vec3 t_i = glm::vec3(target - secondToLast->getModelPosition()); //odejmowanie w dobrym kierunku
                e_i = glm::normalize(e_i);
                t_i = glm::normalize(t_i);
                std::cout << " e_i: " << e_i.x << " " << e_i.y << " " << e_i.z << std::endl;
                std::cout << " t_i: " << t_i.x << " " << t_i.y << " " << t_i.z << std::endl;
                float angle = glm::acos(glm::dot(e_i,t_i)); //kąt wychodzi prawidłowy
                std::cout << "angle: "<< glm::degrees(angle) <<std::endl;
                if(fabs(angle) > 0.01f){
                    glm::vec3 axis = glm::cross(e_i,t_i); //rotation axis
                    std::cout << " axis " << axis.x << " " << axis.y << " " << axis.z << std::endl;
                    if( glm::pow(glm::length(axis),2) > 0.0f){
                        axis = glm::normalize(axis);
                        //obrot macierzy modelSpace
                        glm::mat3 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
                        //glm::mat4 newModelTransform = rotationMatrix * secondToLast->getModelTransform();
                        glm::mat3 newModelTransform = rotationMatrix * glm::mat3(secondToLast->getModelTransform());
                        //!CONSTRAINTS
                        glm::vec3 jointAxis = hingeAxis;
                        jointAxis =  jointAxis * glm::mat3(secondToLast->getModelTransform());
                        jointAxis = glm::normalize(jointAxis);

                        //secondToLast->setModelTransform(newModelTransform);
                        secondToLast->updateModelRotationPart(newModelTransform);
/*
                        //!CONSTRAINTS v2
                        if(secondToLast->getName() == "mixamorig:RightForeArm"){
                            glm::vec3 jointAxisNew = hingeAxis;
                            jointAxisNew = jointAxisNew * glm::mat3(secondToLast->getModelTransform()) ;
                            jointAxisNew = glm::normalize(jointAxisNew);
                            float hepler = glm::dot(jointAxis,jointAxisNew);
                            float angle2 = glm::acos(hepler);
                            glm::vec3 axis2 = glm::cross(jointAxis,jointAxisNew);
                            axis2 = glm::normalize(axis2);
                            glm::mat3 rotationMatrix2 = glm::rotate(glm::mat4(1.0f), angle2, axis2);
                            glm::mat3 newModelTransform2 = rotationMatrix2 * glm::mat3(secondToLast->getModelTransform());
                            secondToLast->updateModelRotationPart(newModelTransform2);
                        }
*/
                        //przygotowanie macierzy odwrotnej do macierzy rotacji rodzica
                        glm::mat4 parentModelMatrix = secondToLast->getParent()->getModelTransform();
                        glm::mat3 parentRotationPart = glm::mat3(parentModelMatrix);
                        glm::mat3 invParentRotationPart = glm::inverse(parentRotationPart); //odwrotnoscc macierzy oblicza sie prawidlowo

                        //dopasowanie localSpace na podstawie zmian wprowadzonych w modelSpace
                       // glm::mat4 newLocalMatrix = glm::mat4(invParentRotationPart) * secondToLast->getModelTransform(); //oblicza sie prawidlowo
                        //secondToLast->setLocalTransform(newLocalMatrix);
                        glm::mat3 newLocalMatrix = invParentRotationPart * glm::mat3(secondToLast->getModelTransform()); //oblicza sie prawidlowo
                        secondToLast->updateLocalRotationPart(newLocalMatrix);


                        updateChildren(secondToLast);
                        endEffector = limbBone ->getModelPosition();
                        //!DEBUG
                        if(secondToLast->getName() == "mixamorig:RightArm") {
                            std::cout << "Bone" << secondToLast->getName() << std::endl;
                            std::cout << "Model matrix: " << std::endl;
                            std::cout << secondToLast->getModelTransform()[0][0] << " "
                                      << secondToLast->getModelTransform()[1][0] << " "
                                      << secondToLast->getModelTransform()[2][0] << " "
                                      << secondToLast->getModelTransform()[3][0] << std::endl;
                            std::cout << secondToLast->getModelTransform()[0][1] << " "
                                      << secondToLast->getModelTransform()[1][1] << " "
                                      << secondToLast->getModelTransform()[2][1] << " "
                                      << secondToLast->getModelTransform()[3][1] << std::endl;
                            std::cout << secondToLast->getModelTransform()[0][2] << " "
                                      << secondToLast->getModelTransform()[1][2] << " "
                                      << secondToLast->getModelTransform()[2][2] << " "
                                      << secondToLast->getModelTransform()[3][2] << std::endl;
                            std::cout << secondToLast->getModelTransform()[0][3] << " "
                                      << secondToLast->getModelTransform()[1][3] << " "
                                      << secondToLast->getModelTransform()[2][3] << " "
                                      << secondToLast->getModelTransform()[3][3] << std::endl;
                            std::cout << "Local matrix: " << std::endl;
                            std::cout << secondToLast->getLocalTransform()[0][0] << " "
                                      << secondToLast->getLocalTransform()[1][0] << " "
                                      << secondToLast->getLocalTransform()[2][0] << " "
                                      << secondToLast->getLocalTransform()[3][0] << std::endl;
                            std::cout << secondToLast->getLocalTransform()[0][1] << " "
                                      << secondToLast->getLocalTransform()[1][1] << " "
                                      << secondToLast->getLocalTransform()[2][1] << " "
                                      << secondToLast->getLocalTransform()[3][1] << std::endl;
                            std::cout << secondToLast->getLocalTransform()[0][2] << " "
                                      << secondToLast->getLocalTransform()[1][2] << " "
                                      << secondToLast->getLocalTransform()[2][2] << " "
                                      << secondToLast->getLocalTransform()[3][2] << std::endl;
                            std::cout << secondToLast->getLocalTransform()[0][3] << " "
                                      << secondToLast->getLocalTransform()[1][3] << " "
                                      << secondToLast->getLocalTransform()[2][3] << " "
                                      << secondToLast->getLocalTransform()[3][3] << std::endl;
                        }
                    }

                }
                secondToLast = secondToLast->getParent();
            }
        }
        prevOffset = offset;
    }
    /*
    ///IK Z KWATERNIONAMI, to nie jest dobrze
    void ik(std::string limb){
        glm::vec3 target = glm::vec3(-124, 688, -23);
        Bone* limbBone;
        if(bones.find(limb)!=bones.end()){ //searching for limb in our bones
            limbBone = bones[limb];
        }
        glm::vec3 endEffector = limbBone ->getModelPosition();
        std::cout << "NOWA KLATKA"<<std::endl;
        for(int j = 0; j <1; j++){ //petla by zwiekszyc dokladnosc wyniku
            Bone* secondToLast = limbBone->getParent();
            for (int i =0; i <3; i++) { //petla by przejsc 3 poprzednie kosci
                glm::vec3 e_i = glm::vec3(endEffector - secondToLast->getModelPosition());
                glm::vec3 t_i = glm::vec3(target - secondToLast->getModelPosition()); //odejmowanie w dobrym kierunku
                e_i = glm::normalize(e_i);
                t_i = glm::normalize(t_i);
                float angle = glm::acos(glm::dot(e_i,t_i)); //kąt wychodzi prawidłowy
                if(fabs(angle) >0.01f){
                    glm::vec3 axis = glm::cross(e_i,t_i); //rotation axis
                    axis = glm::normalize(axis);

                    glm::quat rotationQuat = glm::angleAxis(angle, axis);
                    glm::mat4 mat = secondToLast->getModelTransform() * glm::toMat4(rotationQuat);
                    secondToLast->setModelTransform(mat);


                    glm::mat4 parentModelMatrix = secondToLast->getParent()->getModelTransform();
                    glm::quat parentRotationQuat = glm::quat_cast(glm::mat3(parentModelMatrix));
                    glm::quat invParentRotationQuat = glm::inverse(parentRotationQuat);
                    glm::quat localQuat = glm::quat_cast(secondToLast->getModelTransform());


                    glm::quat newLocalQuat = invParentRotationQuat * localQuat;

                    // Adjust the local matrix using the inverted parent rotation part
                    glm::mat4 newLocalMatrix =  glm::mat4(newLocalQuat) * secondToLast->getModelTransform();
                    secondToLast->setLocalTransform(newLocalMatrix);

                    updateChildren(secondToLast);
                    endEffector = limbBone ->getModelPosition();
                    std::cout <<  endEffector.x << " " <<endEffector.y << " "<<endEffector.z << std::endl;
                }
                secondToLast = secondToLast->getParent();
            }
        }

    }*/
/*
//ik lokalnie
    void ik(std::string limb, int offset){
        Bone* limbBone;
        if(bones.find(limb)!=bones.end()){ //searching for limb in our bones
            limbBone = bones[limb];
        }
        glm::vec3 target = limbBone->getLocalPosition();
        if(offset != prevOffset){
            target = target + vec3(offset/10, offset/10,0);
        }
        std::cout << offset <<std::endl;
        glm::vec3 endEffector = limbBone ->getLocalPosition();
        std::cout <<  endEffector.x << " " <<endEffector.y << " "<<endEffector.z << std::endl;
        std::cout << "NOWA KLATKA"<<std::endl;
        for(int j = 0; j <10; j++){ //petla by zwiekszyc dokladnosc wyniku
            Bone* secondToLast = limbBone->getParent(); //przypisanie przedramienia jako kosci ktora manewrujemy jako pierwsza
            for (int i =0; i <3; i++) { //petla by przejsc 3 poprzednie kosci
                glm::vec3 e_i = glm::vec3(endEffector - secondToLast->getLocalPosition());
                glm::vec3 t_i = glm::vec3(target - secondToLast->getLocalPosition()); //odejmowanie w dobrym kierunku
                e_i = glm::normalize(e_i);
                t_i = glm::normalize(t_i);
                float angle = glm::acos(glm::dot(e_i,t_i)); //kąt wychodzi prawidłowy
                if(fabs(angle) >0.01f){
                    glm::vec3 axis = glm::cross(e_i,t_i); //rotation axis
                    axis = glm::normalize(axis);
                    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
                    glm::mat4 newLocalTransform = rotationMatrix * secondToLast->getLocalTransform();
                    secondToLast->setLocalTransform(newLocalTransform);

                    //dopasowanie localSpace na podstawie zmian wprowadzonych w modelSpace
                    glm::mat4 newModelMatrix =secondToLast->getLocalTransform() * secondToLast->getParent()->getModelTransform();
                    secondToLast->setModelTransform(newModelMatrix);

                    updateChildren(secondToLast);

                }
                secondToLast = secondToLast->getParent();
            }
            endEffector = limbBone ->getLocalPosition();
        }
        prevOffset = offset;
    }*/

    void updateChildren(Bone* parent) {
        for(Bone* child : parent->getChildren()) {
            // Update the child bone's transformation
            glm::mat4 modelSpaceTransformation = parent->getModelTransform() * child->getLocalTransform();
            child->setModelTransform(modelSpaceTransformation);
            updateChildren(child); // Recursively update children
        }
    }

private:
    Bone* rootBone;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
    std::vector<glm::mat4> m_FinalBoneMatrices;
    std::map<std::string,Bone*> bones;
    Model* rigsModel;
    int prevOffset;
    glm::vec3 hingeAxis;
    glm::vec3 initTarget;
};
#endif //ENGINE_RIGPREP_H
