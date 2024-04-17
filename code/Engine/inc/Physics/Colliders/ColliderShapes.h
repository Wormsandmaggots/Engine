//
// Created by Radek on 08.04.2024.
//

#ifndef ENGINE_COLLIDERSHAPES_H
#define ENGINE_COLLIDERSHAPES_H


#include "glm/vec3.hpp"
#include "glm/geometric.hpp"

namespace ColliderShapes {
    enum class ColliderShapeType {
        SPHERE,
        BOX
    };

    struct AABB {
        glm::vec3 min;
        glm::vec3 max;
    };

    struct Shape {
        glm::vec3 position{0.f};
        glm::vec3 offset{0.f};
        glm::vec3 size{0.f};

        Shape(const glm::vec3& position, const glm::vec3& offset, const glm::vec3& size = glm::vec3(0.f)):
        position(position), offset(offset), size(size) {}

        virtual ~Shape() = default;

        [[nodiscard]] virtual AABB getAABB() const = 0;
        [[nodiscard]] virtual float getRadius() const = 0;
        [[nodiscard]] virtual ColliderShapeType getType() const = 0;
        virtual void setRadius(float r) {};
        virtual void draw() {};
    };

    struct Sphere : public Shape {
        float radius = 0.f;

        Sphere(const glm::vec3& position, const glm::vec3& offset, float radius)
                : Shape(position, offset), radius(radius) {}

        ~Sphere() override = default;

        [[nodiscard]] AABB getAABB() const override {
            return {
                    position - radius * glm::vec3(1.f),
                    position + radius * glm::vec3(1.f)};
        }

        [[nodiscard]] float getRadius() const override {
            return radius;
        }

        void setRadius(float r) override{
            radius = r;
        }

        [[nodiscard]] ColliderShapeType getType() const override {
            return ColliderShapeType::SPHERE;
        }
    };

    struct Box : public Shape {
        const float boundingCircleRadius;

        Box(const glm::vec3& position, const glm::vec3& offset, const glm::vec3& size)
                : Shape(position, offset, size), boundingCircleRadius(glm::length(size) * 0.5f) {}

        ~Box() override = default;

        [[nodiscard]] AABB getAABB() const override {
            return {
                    position - size * glm::vec3(1.f) * 0.5f,
                    position + size * glm::vec3(1.f) * 0.5f};
        }

        [[nodiscard]] float getRadius() const override {
            return boundingCircleRadius;
        }

        [[nodiscard]] ColliderShapeType getType() const override {
            return ColliderShapeType::BOX;
        }

        [[nodiscard]] float getTop() const {
            return position.y + (size.y * 0.5f);
        }

        [[nodiscard]] float getBottom() const {
            return position.y - (size.y * 0.5f);
        }

        [[nodiscard]] float getLeft() const {
            return position.x - (size.x * 0.5f);
        }

        [[nodiscard]] float getRight() const {
            return position.x + (size.x * 0.5f);
        }

        [[nodiscard]] float getForward() const {
            return position.z + (size.z * 0.5f);
        }

        [[nodiscard]] float getBack() const {
            return position.z - (size.z * 0.5f);
        }
    };
}


#endif //ENGINE_COLLIDERSHAPES_H
