#ifndef ENGINE_BONE_H
#define ENGINE_BONE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Bone{
private:
    Bone* parent;
    std::vector<Bone*> children;
    std::string name;
    glm::mat4 localTransform;
    glm::mat4 modelTransform;
    int childrenCount;
public:
    Bone(){}

    Bone* getParent(){return parent;}
    std::vector<Bone*>& getChildren(){return children;}
    std::string getName(){return name;}
    glm::mat4 getLocalTransform(){return localTransform;}
    glm::mat4 getModelTransform(){return modelTransform;}
    int getChildrenCount(){return childrenCount;}
    glm::vec3 getLocalPosition(){return glm::vec3(localTransform[3]);}
    glm::vec3 getModelPosition(){return glm::vec3(modelTransform[3]);}

    void setParent(Bone* p){parent = p;}
    void setChildren(std::vector<Bone*> c){children = c;}
    void setName(std::string n){name = n;}
    void setLocalTransform(glm::mat4 t){ localTransform = t;}
    void setModelTransform(glm::mat4 t){ modelTransform = t;}
    void setChildrenCount(int c){childrenCount =c;}
    void updateModelRotationPart(glm::mat3 newRotation) {
        modelTransform[0][0] = newRotation[0][0];
        modelTransform[0][1] = newRotation[0][1];
        modelTransform[0][2] = newRotation[0][2];
        modelTransform[1][0] = newRotation[1][0];
        modelTransform[1][1] = newRotation[1][1];
        modelTransform[1][2] = newRotation[1][2];
        modelTransform[2][0] = newRotation[2][0];
        modelTransform[2][1] = newRotation[2][1];
        modelTransform[2][2] = newRotation[2][2];
    }
    void updateLocalRotationPart(glm::mat3 newRotation) {
        localTransform[0][0] = newRotation[0][0];
        localTransform[0][1] = newRotation[0][1];
        localTransform[0][2] = newRotation[0][2];
        localTransform[1][0] = newRotation[1][0];
        localTransform[1][1] = newRotation[1][1];
        localTransform[1][2] = newRotation[1][2];
        localTransform[2][0] = newRotation[2][0];
        localTransform[2][1] = newRotation[2][1];
        localTransform[2][2] = newRotation[2][2];
    }
};
#endif //ENGINE_BONE_H
