#ifndef OPENGLGP_SCENE_H
#define OPENGLGP_SCENE_H

#include "Transform.h"

#include <vector>

class [[deprecated("This scene is deprecated, use one from Scene folder.")]] Scene
{
public:
    Scene() {}
    ~Scene() {}

    void addObjects(Transform* transform)
    {
        sceneObjects.push_back(transform);
    }

    //it can be passed through reference
    void UpdateTransform(Shader defaultShader)
    {
        for (Transform* t : sceneObjects) {
            t->updateWorldTransform(new glm::mat4(1.f), &defaultShader);
        }
    }

private:
    std::vector<Transform*> sceneObjects;

};


#endif //OPENGLGP_SCENE_H
