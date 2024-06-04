#include "glm/vec3.hpp"
#include "RigPrep.h"

class LookAt{
private:
    std::map<std::string,Bone*> bones;
    RigPrep* rig;
public:
    LookAt(RigPrep* _rig){
        rig = _rig;
        bones = rig->getBones();
    }

    void update(){}
    void compute(){}
    void updateChildren(Bone* parent) {
        for(Bone* child : parent->getChildren()) {
            // Update the child bone's transformation
            glm::mat4 modelSpaceTransformation = parent->getModelTransform() * child->getLocalTransform();
            child->setModelTransform(modelSpaceTransformation);
            updateChildren(child); // Recursively update children
        }
    }

};
