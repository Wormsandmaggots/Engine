#include "glm/vec3.hpp"
#include "RigPrep.h"
class LookAt{
private:
    std::map<std::string,Bone*> bones;
    RigPrep* rig;
    glm::vec3 forward;
    glm::mat4 headInitPos;
public:
    LookAt(RigPrep* _rig){
        rig = _rig;
        bones = rig->getBones();
        forward = glm::vec3(0.0f, 0.0f, 1.0f); //wektor do przodu, defaultowo glowa patrzy w tym kierunku
        Bone* head;
        if(bones.find("mixamorig:Head")!=bones.end()){ //searching for limb in our bones
            head = bones["mixamorig:Head"];
        }
        headInitPos = head->getModelTransform();
    }
///tej funkcji używamy gdy chcemy podać punkt na który ma patrzeć postać (w odniesieniu do glowy postaci)
    void update(glm::vec3 target){
        compute(target);
    }
    /// tej funkcji uzywamy gdy chcemy podać kąt o jaki głowa ma zostać obrócona względem jej początkowego zwrotu
    void update(float angle){
        compute(angle);
    }

    void compute(glm::vec3 pointTarget){
        Bone* head;
        if(bones.find("mixamorig:Head")!=bones.end()){ //searching for limb in our bones
            head = bones["mixamorig:Head"];
        }
        glm::vec3 headPos = head ->getModelPosition();
        glm::vec3 target = glm::vec3(headPos.x + pointTarget.x, headPos.y + pointTarget.y, headPos.z + pointTarget.z);
        if(forward == glm::vec3(0.0f,0.0f,1.0f)){
            forward = glm::vec3(headPos.x , headPos.y, headPos.z + 10);
        }
        glm::vec3 f_h = glm::vec3(forward-headPos); //forward to head
        glm::vec3 t_h = glm::vec3(target-headPos); //target to head
        f_h = glm::normalize(f_h);
        t_h = glm::normalize(t_h);
        float angle = glm::acos(glm::dot(f_h, t_h));
        LOG_INFO("ANGLE: " + std::to_string(angle));
        if(fabs(angle) > 0.001f){
            glm::vec3 axis = glm::cross(f_h, t_h);
            if( glm::pow(glm::length(axis),2) > 0.0f){
                axis = glm::normalize(axis);
                //obrot macierzy w modelspace
                glm::mat3 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
                glm::mat3 newModelTransform = rotationMatrix * glm::mat3(head->getModelTransform());
                head->updateModelRotationPart(newModelTransform);
                //przygotowanie macierzy odwrotnej do macierzy rotacji rodzica
                glm::mat4 parentModelMatrix = head->getParent()->getModelTransform();
                glm::mat3 parentRotationPart = glm::mat3(parentModelMatrix);
                glm::mat3 invParentRotationPart = glm::inverse(parentRotationPart);

                //dopasowanie localSpace na podstawie zmian wprowadzonych w modelSpace
                glm::mat3 newLocalMatrix = invParentRotationPart * glm::mat3(head->getModelTransform()); //oblicza sie prawidlowo
                head->updateLocalRotationPart(newLocalMatrix);

                forward = rotationMatrix * forward;
                updateChildren(head);
            }
        }
    }

    void compute(float angle){
        Bone* head;
        if(bones.find("mixamorig:Head")!=bones.end()){ //searching for limb in our bones
            head = bones["mixamorig:Head"];
        }
        if(fabs(angle) > 0.001f){
            glm::vec3 axis = glm::vec3(0.0f,1.0f,0.0f);
                //obrot macierzy w modelspace
                glm::mat3 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
                glm::mat3 newModelTransform = rotationMatrix * glm::mat3(headInitPos);
                head->updateModelRotationPart(newModelTransform);
                //przygotowanie macierzy odwrotnej do macierzy rotacji rodzica
                glm::mat4 parentModelMatrix = head->getParent()->getModelTransform();
                glm::mat3 parentRotationPart = glm::mat3(parentModelMatrix);
                glm::mat3 invParentRotationPart = glm::inverse(parentRotationPart);

                //dopasowanie localSpace na podstawie zmian wprowadzonych w modelSpace
                glm::mat3 newLocalMatrix = invParentRotationPart * glm::mat3(head->getModelTransform()); //oblicza sie prawidlowo
                head->updateLocalRotationPart(newLocalMatrix);

                updateChildren(head);
            }
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