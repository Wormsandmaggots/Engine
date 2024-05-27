#include "glm/vec3.hpp"
#include "RigPrep.h"

class InverseKinematics{
private:
    glm::vec3 hingeAxis;
    glm::vec3 initTarget;
    std::map<std::string,Bone*> bones;
    RigPrep* rig;

public:
    InverseKinematics(RigPrep* _rig){
        rig = _rig;
        bones = rig->getBones();
        hingeAxis = glm::vec3(0.0f,1.0f,0.0f);
        //initTarget = glm::vec3(-138.593, 288.838, -4.86769);
    }
    void update(int offset, string _limb){
        ik(_limb,offset);
        rig->setBones(bones);
    }

    void ik(std::string limb, int offset){
        Bone* limbBone;
        if(bones.find(limb)!=bones.end()){ //searching for limb in our bones
            limbBone = bones[limb];
        }
        //glm::vec3 target = limbBone->getModelPosition();

        glm::vec3 target = limbBone->getInitPosition() + glm::vec3(offset, offset, 0);
        //glm::vec3 target = initTarget;
        glm::vec3 endEffector = limbBone ->getModelPosition();
        for(int j = 0; j <10; j++){ //petla by zwiekszyc dokladnosc wyniku
            Bone* secondToLast = limbBone->getParent(); //przypisanie przedramienia jako kowsci ktora manewrujemy jako pierwsza
            for (int i =0; i <2; i++) { //petla by przejsc 3 poprzednie kosci
                glm::vec3 e_i = glm::vec3(endEffector - secondToLast->getModelPosition());
                glm::vec3 t_i = glm::vec3(target - secondToLast->getModelPosition()); //odejmowanie w dobrym kierunku
                e_i = glm::normalize(e_i);
                t_i = glm::normalize(t_i);
                float angle = glm::acos(glm::dot(e_i,t_i)); //kąt wychodzi prawidłowy
                if(fabs(angle) > 0.001f){
                    glm::vec3 axis = glm::cross(e_i,t_i); //rotation axis
                    std::cout << " axis " << axis.x << " " << axis.y << " " << axis.z << std::endl;
                    if( glm::pow(glm::length(axis),2) > 0.0f){
                        axis = glm::normalize(axis);
                        //obrot macierzy modelSpace
                        glm::mat3 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
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
                        glm::mat3 newLocalMatrix = invParentRotationPart * glm::mat3(secondToLast->getModelTransform()); //oblicza sie prawidlowo
                        secondToLast->updateLocalRotationPart(newLocalMatrix);

                        updateChildren(secondToLast);
                        endEffector = limbBone ->getModelPosition();
                    }

                }
                secondToLast = secondToLast->getParent();
            }
        }
        //prevOffset = offset;
    }

    void updateChildren(Bone* parent) {
        for(Bone* child : parent->getChildren()) {
            // Update the child bone's transformation
            glm::mat4 modelSpaceTransformation = parent->getModelTransform() * child->getLocalTransform();
            child->setModelTransform(modelSpaceTransformation);
            updateChildren(child); // Recursively update children
        }
    }
};