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
};
#endif //ENGINE_BONE_H
