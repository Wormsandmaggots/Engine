//
// Created by Radek on 08.04.2024.
//

#ifndef ENGINE_COLLISIONMANAGER_H
#define ENGINE_COLLISIONMANAGER_H


#include <vector>

class Collider;

enum CollisionType{
    START,
    EXIT,
    REMAIN
};

struct Collision {
    Collider* first;
    Collider* second;
    CollisionType firstType;
    CollisionType secondType;
};

class CollisionManager {
public:
    static void AddCollider(Collider*);
    static void RemoveCollider(Collider*);

    CollisionManager() = default;
    virtual ~CollisionManager() = default;

    void update();
    void resolveCollisions();
private:
    static std::vector<Collider*> Colliders;
    std::vector<Collision> collisions;
};


#endif //ENGINE_COLLISIONMANAGER_H
