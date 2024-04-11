//
// Created by Radek on 08.04.2024.
//

#ifndef ENGINE_COLLISIONDETECTION_H
#define ENGINE_COLLISIONDETECTION_H

#include "glm/vec3.hpp"

namespace ColliderShapes {
    class Box;
    class Sphere;
}

class CollisionDetection {
public:
    static bool CircleCircleCollision(
             ColliderShapes::Sphere* circleOne,
             ColliderShapes::Sphere* circleTwo);

    static glm::vec3 CircleCircleSeparation(
             ColliderShapes::Sphere* circleOne,
             ColliderShapes::Sphere* circleTwo);

    static bool RectangleRectangleCollision(
             ColliderShapes::Box* rectangleOne,
             ColliderShapes::Box* rectangleTwo);

    static glm::vec3 RectangleRectangleSeparation(
             ColliderShapes::Box* rectangleOne,
             ColliderShapes::Box* rectangleTwo);

    static bool RectangleCircleCollision(
             ColliderShapes::Box* rectangle,
             ColliderShapes::Sphere* circle);

    static glm::vec3 RectangleCircleSeparation(
             ColliderShapes::Box* rectangle,
             ColliderShapes::Sphere* circle);

    static glm::vec3 CalculateNearestPointOnRect(
             glm::vec3& position,
             ColliderShapes::Box* rectangle);

    static glm::vec3 FindCollisionPointForCircle(
             ColliderShapes::Sphere* circle,
             glm::vec3& anotherPosition);

    static glm::vec3 FindCollisionPointForRect(
             ColliderShapes::Box* rectangle,
             glm::vec3& anotherPosition);
};


#endif //ENGINE_COLLISIONDETECTION_H
