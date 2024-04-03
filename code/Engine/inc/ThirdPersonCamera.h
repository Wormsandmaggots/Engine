#ifndef ENGINE_THIRDPERSONCAMERA_H
#define ENGINE_THIRDPERSONCAMERA_H

#include "ECS/Component.h"
#include <string>

class ThirdPersonCamera : public Component{
public:
    ~ThirdPersonCamera() override = default;

    void awake() override;

    void start() override;

    void update() override;

    void onDestroy() override;

    void setParent(Entity *entity) override;

    std::string serialize() override;
};
#endif //ENGINE_THIRDPERSONCAMERA_H
