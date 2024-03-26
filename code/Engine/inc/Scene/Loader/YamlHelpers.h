//
// Created by Radek on 24.03.2024.
//

#ifndef ENGINE_YAMLHELPERS_H
#define ENGINE_YAMLHELPERS_H

#include "yaml-cpp/yaml.h"

#include "Transform_old.h"
#include "ECS/Entity.h"
#include "Debug/Logger.h"
#include "Scene/Transform2.h"


namespace YAML {
    template<>
    struct convert<vec3>
    {
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
    struct convert<Transform_old> {
        static Node encode(const Transform_old &rhs) {
            Node node;
            node.push_back(rhs.position);
            node.push_back(rhs.rotateModel);
            node.push_back(rhs.scale);
            return node;
        }

        static bool decode(const Node &node, Transform_old &rhs) {
            rhs.position = node["pos"].as<vec3>();
            rhs.rotateModel = node["rot"].as<vec3>();
            rhs.scale = node["scale"].as<vec3>();
            return true;
        }
    };

    template<>
    struct convert<Transform2> {
        static Node encode(const Transform2 &rhs) {
            Node node;
            node.push_back(rhs.getLocalPosition());
            node.push_back(rhs.getLocalRotation());
            node.push_back(rhs.getLocalScale());
            return node;
        }

        static bool decode(const Node &node, Transform2 &rhs) {
            rhs.setPosition(node["pos"].as<vec3>());
            rhs.setRotation(node["rot"].as<vec3>());
            rhs.setScale(node["scale"].as<vec3>());
            return true;
        }
    };

    //!THIS TEMPLATE SHOULD BE IMPLEMENTED FOR EVERY COMPONENT
    template<>
    struct convert<Component> {
        static Node encode(const Component &rhs) {
            Node node;

            return node;
        }

        static bool decode(const Node &node, Component &rhs) {

            return true;
        }
    };

    template<>
    struct convert<Entity> {
        static Node encode(const Entity &rhs) {
            Node node;
            node.push_back(rhs.getName());
            node.push_back(*rhs.getTransform());
            return node;
        }

        static bool decode(const Node &node, Entity &rhs) {
            rhs.setName(node["Name"].as<string>());
            rhs.setTransform(node["Transform_old"].as<Transform2>());
            return true;
        }
    };
}
#endif //ENGINE_YAMLHELPERS_H
