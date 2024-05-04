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
    glm::mat4 transform;
    int childrenCount;
public:
    Bone(){}

    Bone* getParent(){return parent;}
    std::vector<Bone*>& getChildren(){return children;}
    std::string getName(){return name;}
    glm::mat4 getTransform(){return transform;}
    int getChildrenCount(){return childrenCount;}

    void setParent(Bone* p){parent = p;}
    void setChildren(std::vector<Bone*> c){children = c;}
    void setName(std::string n){name = n;}
    void setTransform( glm::mat4 t){transform = t;}
    void setChildrenCount(int c){childrenCount =c;}
};
#endif //ENGINE_BONE_H
