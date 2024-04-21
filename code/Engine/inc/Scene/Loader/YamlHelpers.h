//
// Created by Radek on 24.03.2024.
//

#ifndef ENGINE_YAMLHELPERS_H
#define ENGINE_YAMLHELPERS_H

#include "yaml-cpp/yaml.h"
#include "ECS/Entity.h"
#include "Debug/Logger.h"
#include "Scene/Transform.h"
#include "Scene/Scene.h"
#include "Physics/ColliderComponent.h"

using namespace glm;
namespace YAML {

    struct tempStruct{
        std::unordered_map<std::string, std::function<void (Entity*, const Node&)>> components  = {
                {"ColliderComponent", [](Entity* e, const Node& element) {
                    e->addComponent(new ColliderComponent(element.as<ColliderComponent>()));}},
                {"Model", [](Entity* e, const Node& element) {
                    e->addComponent<Model>(element["Path"].as<std::string>());}}
        };
    };

    template<>
    struct convert<vec2>{
        static Node encode(const vec2 &rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node &node, vec2 &rhs) {
            if (!node.IsSequence()) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };


    template<>
    struct convert<vec3>{
        static Node encode(const vec3 &rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node &node, vec3 &rhs) {
            if (!node.IsSequence()) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<Transform> {
        static Node encode(const Transform &rhs) {
            Node node;
            node.push_back(rhs.getLocalPosition());
            node.push_back(rhs.getLocalRotation());
            node.push_back(rhs.getLocalScale());
            return node;
        }

        static bool decode(const Node &node, Transform &rhs) {
            rhs.setPosition(node["pos"].as<vec3>());
            rhs.setRotation(node["rot"].as<vec3>());
            rhs.setScale(node["scale"].as<vec3>());
            return true;
        }
    };

    template<>
    struct convert<Entity> {
        static Node encode(const Entity &rhs) {
            Node node;
            node.push_back(rhs.getName());
            node.push_back(*rhs.getTransform());
            for (Component* c : rhs.getComponents()) {

            }
            return node;
        }

        static bool decode(const Node &node, Entity &rhs) {
            rhs.setName(node["Name"].as<string>());
            rhs.setTransform(node["Transform"].as<Transform>());

            tempStruct thisShit;

            for (const auto& element : node["Components"]) {
                if(!element["Type"])
                {
                    LOG_WARNING("COMPONENT THAT IS ATTACHED TO " + rhs.getName() + " DOES NOT PROVIDE A TYPE MEMBER");
                }
                else
                {
                    thisShit.components.at(element["Type"].as<std::string>())(&rhs, element);
                }
            }
            return true;
        }
    };

    template<>
    struct convert<ColliderComponent> {
        static Node encode(const ColliderComponent &rhs) {
            Node node;

            return node;
        }

        static bool decode(const Node &node, ColliderComponent &rhs) {

            rhs.getCollider()->setColliderType(static_cast<ColliderType>(node["ColliderType"].as<int>()));
            if(node["Radius"])
            {
                rhs.getCollider()->setColliderShape(new ColliderShapes::Sphere(glm::vec3(0.f), node["Offset"].as<vec3>(), node["Radius"].as<float>()));
            }
            else
            {
                rhs.getCollider()->setColliderShape(new ColliderShapes::Box(glm::vec3(0.f), node["Offset"].as<vec3>(), node["Size"].as<vec3>()));
            }

            rhs.start();

            return true;
        }
    };

    template<>
    struct convert<Model> {
        static Node encode(const Model &rhs) {
            Node node;
            return node;
        }

        static bool decode(const Node &node, Model &rhs) {

            rhs.setPath(node["Path"].as<std::string>());

            return true;
        }
    };
}
#endif //ENGINE_YAMLHELPERS_H
