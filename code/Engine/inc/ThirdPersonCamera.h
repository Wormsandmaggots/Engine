///implementation explained: https://www.youtube.com/watch?v=PoxDDZmctnU
#ifndef ENGINE_THIRDPERSONCAMERA_H
#define ENGINE_THIRDPERSONCAMERA_H

#include "ECS/Component.h"
#include "Scene/Transform2.h"
#include <string>
#include "ECS/Entity.h"

class ThirdPersonCamera : public Component{
private:
    //Transform2* transform = new Transform2;
    glm::vec3 localPosition;
    glm::vec3 rotation; //(roll,pitch,yaw)
    glm::vec3 cameraTarget;
    glm::vec3 worldUp;
    glm::mat4 view;
    float zoom = 45.0f;
    float distanceFromPlayer = 5.0f;
    float calculateHorizontalDistance();
    float calculateVerticalDistance();
    void calculateCameraPosition();
public:
    ThirdPersonCamera();
    ~ThirdPersonCamera() override = default;

    void awake() override;

    void start() override;

    void update() override;

    void onDestroy() override;

    void setParent(Entity *entity) override;

    std::string serialize() override;
    void setTransform(Transform2*) override;
    glm::mat4 getView();
    glm::mat4 getProjection(float width, float height);
};
#endif //ENGINE_THIRDPERSONCAMERA_H
