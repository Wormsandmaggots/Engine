//
// Created by Radek on 10.04.2024.
//

#include "Physics/ColliderComponent.h"
#include "Debug/Logger.h"
#include "Scene/Transform2.h"
#include "Editor/Gizmos.h"

void ColliderComponent::start() {
    collider->setOwner(this);
    collider->setOnCollision([this](ColliderComponent* collidedWith){
        onCollision(collidedWith);
    });
    collider->setOnCollisionExit([this](ColliderComponent* collidedWith){
        onCollisionExit(collidedWith);
    });
    collider->setOnCollisionStart([this](ColliderComponent* collidedWith){
        onCollisionStart(collidedWith);
    });
}

void ColliderComponent::onCollision(ColliderComponent *another) {
    
}

void ColliderComponent::update() {
    collider->getColliderShape()->position = parentTransform->getWorldMatrix()[3];
}

void ColliderComponent::onCollisionExit(ColliderComponent *another) {
}

void ColliderComponent::onCollisionStart(ColliderComponent *) {
}

Collider *ColliderComponent::getCollider() const 
{
        return collider;
}

void ColliderComponent::convertToYaml(YAML::Emitter &emitter) {
    float* off = glm::value_ptr(collider->getColliderShape()->offset);
    emitter << YAML::Key << "Type" << YAML::Value << "ColliderComponent";
    emitter << YAML::Key << "ColliderType" << YAML::Value << collider->getColliderType();
    emitter << YAML::Key << "Offset" << YAML::Flow << YAML::BeginSeq << off[0] << off[1] << off[2] << YAML::EndSeq;

    if(collider->getColliderShape()->getType() == ColliderShapes::ColliderShapeType::SPHERE)
    {
        emitter << YAML::Key << "Radius" << YAML::Value << collider->getColliderShape()->getRadius();
    }
    else
    {
        float* size = glm::value_ptr(collider->getColliderShape()->size);
        emitter << YAML::Key << "Size" << YAML::Flow << YAML::BeginSeq  << size[0] << size[1] << size[2] << YAML::EndSeq;
    }
}

void ColliderComponent::drawEditor() {
    ImGui::Text("Collider component");

    const char* types[2] = {"Box", "Sphere"};
    

    static int currentItem = collider->getColliderShape()->getType() == ColliderShapes::ColliderShapeType::BOX ? 0 : 1;

    if(ImGui::Combo("Collider type", &currentItem,
                 types, IM_ARRAYSIZE(types)))
    {
        if(collider->getColliderShape()->getType() == ColliderShapes::ColliderShapeType::BOX && currentItem == 1)
        {
            collider->setColliderShape(new ColliderShapes::Sphere(glm::vec3(0.f), glm::vec3(0.f), 0.7102f));
        }
        else if(collider->getColliderShape()->getType() == ColliderShapes::ColliderShapeType::SPHERE && currentItem == 0)
        {
            collider->setColliderShape(new ColliderShapes::Box(glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f)));
        }
    }

    if(collider->getColliderShape()->getType() == ColliderShapes::ColliderShapeType::BOX)
    {
        ImGui::DragFloat3("Size", glm::value_ptr(collider->getColliderShape()->size), 0.1f);
    } else{
        static float radius = collider->getColliderShape()->getRadius();

        ImGui::DragFloat("Radius", &radius, 0.1f);

        collider->getColliderShape()->setRadius(radius);
    }

    ImGui::NewLine();
}

//ColliderComponent::~ColliderComponent() {
//    delete collider;
//}
