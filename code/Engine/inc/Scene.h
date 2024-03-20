#ifndef OPENGLGP_SCENE_H
#define OPENGLGP_SCENE_H

#include "Transform.h"
#include <vector>

class Scene
{
private:
    std::vector<Transform*> sceneObjects;

public:
    Scene() {}
    ~Scene() {}

    void addObjects(Transform* transform)
    {
        sceneObjects.push_back(transform);
    }

    void UpdateTransform(Shader shader)
    {
        for (Transform* t : sceneObjects) {
            t->updateWorldTransform(new glm::mat4(1.f), &shader);
        }
    }
};


#endif //OPENGLGP_SCENE_H
