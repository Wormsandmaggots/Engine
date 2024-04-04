#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera() {
    localPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, -90.0f); //(roll,pitch,yaw)
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    //transform = new Transform2(localPosition, rotation, glm::vec3(1.0f,1.0f,1.0f));
    view = glm::lookAt(localPosition,cameraTarget, worldUp);
}

void ThirdPersonCamera::awake() {

}

void ThirdPersonCamera::start() {

}

void ThirdPersonCamera::update() {
    //transform->setPosition(parent->getTransform()->getLocalPosition());
    cameraTarget = parent->getTransform()->getLocalPosition();
    cameraTarget.y +=2;
    calculateCameraPosition();
    view = glm::lookAt(localPosition,cameraTarget, worldUp);
}

void ThirdPersonCamera::onDestroy() {

}

void ThirdPersonCamera::setParent(Entity *entity) {
    parent = entity;
    //transform->setPosition(parent->getTransform()->getLocalPosition());
    cameraTarget = parent->getTransform()->getLocalPosition();
}

std::string ThirdPersonCamera::serialize() {
    return std::string();
}

float ThirdPersonCamera::calculateHorizontalDistance() {
    return distanceFromPlayer * cos(glm::radians(rotation.y)); //pitch
}

float ThirdPersonCamera::calculateVerticalDistance() {
    return distanceFromPlayer * sin(glm::radians(rotation.y)); //pitch
}

void ThirdPersonCamera::calculateCameraPosition() {
    float theta = parent->getTransform()->getLocalRotation().y; //angle around player
    float offsetX = calculateHorizontalDistance() * sin(glm::radians(theta));
    float offsetZ = calculateHorizontalDistance() * cos(glm::radians(theta));
    localPosition.x = cameraTarget.x - offsetX;
    localPosition.z = cameraTarget.z - offsetZ;
    localPosition.y = cameraTarget.y + calculateVerticalDistance(); //distance up
    rotation.z = 180 -(parent->getTransform()->getLocalRotation().y - theta);
}

glm::mat4 ThirdPersonCamera::getView() {
    return view;
}
void ThirdPersonCamera::setTransform(Transform2* newTransform) {
}
