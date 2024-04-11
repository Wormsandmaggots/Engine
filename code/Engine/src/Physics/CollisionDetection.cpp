//
// Created by Radek on 08.04.2024.
//

#include <memory>
#include "Physics/CollisionDetection.h"
#include "Physics/Colliders/ColliderShapes.h"
#include "Core/Utils/MathUtils.h"

using namespace ColliderShapes;

bool CollisionDetection::CircleCircleCollision( ColliderShapes::Sphere* circleOne,
                                                ColliderShapes::Sphere* circleTwo) {
     float radiusSum = circleOne->radius + circleTwo->radius;
    return glm::length(circleOne->position - circleTwo->position) <= radiusSum * radiusSum;
}

glm::vec3 CollisionDetection::CircleCircleSeparation( ColliderShapes::Sphere* circleOne,
                                                      ColliderShapes::Sphere* circleTwo) {
     glm::vec3 oneToTwoVector = circleTwo->position - circleOne->position;
     float separationLength = circleOne->radius + circleTwo->radius - glm::length(oneToTwoVector);

    return Math::SafeNormal(oneToTwoVector) * separationLength;
}

bool CollisionDetection::RectangleRectangleCollision(ColliderShapes::Box* rectangleOne,
                                                     ColliderShapes::Box* rectangleTwo) {
    return rectangleOne->getRight() > rectangleTwo->getLeft()
           && rectangleOne->getLeft() < rectangleTwo->getRight()
           && rectangleOne->getTop() > rectangleTwo->getBottom()
           && rectangleOne->getBottom() < rectangleTwo->getTop();
}

glm::vec3 CollisionDetection::RectangleRectangleSeparation( ColliderShapes::Box* rectangleOne,
                                                            ColliderShapes::Box* rectangleTwo) {
    float leftSeparation = rectangleOne->getRight() - rectangleTwo->getLeft();
    float rightSeparation = rectangleTwo->getRight() - rectangleOne->getLeft();
    float topSeparation = rectangleOne->getTop() - rectangleTwo->getBottom();
    float bottomSeparation = rectangleTwo->getTop() - rectangleOne->getBottom();

    glm::vec3 finalSeparation;
    finalSeparation.x = leftSeparation < rightSeparation ? -leftSeparation : rightSeparation;
    finalSeparation.y = bottomSeparation < topSeparation ? bottomSeparation : -topSeparation;

    if (std::abs(finalSeparation.x) < std::abs(finalSeparation.y))
        finalSeparation.y = 0.f;
    else
        finalSeparation.x = 0.f;

    return -finalSeparation;
}

bool CollisionDetection::RectangleCircleCollision( ColliderShapes::Box* rectangle,
                                                   ColliderShapes::Sphere* circle) {
    glm::vec3 nearestPoint = CalculateNearestPointOnRect(circle->position, rectangle);
    return glm::length(circle->position - nearestPoint) <= circle->radius;
}

glm::vec3 CollisionDetection::CalculateNearestPointOnRect( glm::vec3& position,
                                                           ColliderShapes::Box* rectangle) {
    glm::vec3 result;
    result.x = glm::clamp(position.x, rectangle->getLeft(), rectangle->getRight());
    result.y = glm::clamp(position.y, rectangle->getBottom(), rectangle->getTop());
    result.z = glm::clamp(position.z, rectangle->getBack(), rectangle->getForward());
    return result;
}

glm::vec3 CollisionDetection::RectangleCircleSeparation( ColliderShapes::Box* rectangle,
                                                         ColliderShapes::Sphere* circle) {
    glm::vec3 nearestPoint = CalculateNearestPointOnRect(circle->position, rectangle);

    auto rectangleAsCircle = std::make_unique<ColliderShapes::Sphere>(nearestPoint, glm::vec3{0.f}, 0.f);

    return CircleCircleSeparation(rectangleAsCircle.get(), circle);
}

glm::vec3 CollisionDetection::FindCollisionPointForCircle( ColliderShapes::Sphere* circle,
                                                           glm::vec3& anotherPosition) {
    glm::vec3 oneToTwoDirection = -Math::SafeNormal(circle->position - anotherPosition);
    return oneToTwoDirection * circle->radius + circle->position;
}

glm::vec3 CollisionDetection::FindCollisionPointForRect( ColliderShapes::Box* rectangle,
                                                         glm::vec3& anotherPosition) {
    return CollisionDetection::CalculateNearestPointOnRect(anotherPosition, rectangle);
}