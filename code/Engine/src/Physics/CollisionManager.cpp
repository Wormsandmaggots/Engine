//
// Created by Radek on 08.04.2024.
//

#include <algorithm>
#include "Physics/CollisionManager.h"
#include "Physics/Colliders/Collider.h"
#include "Physics/ColliderComponent.h"
#include "Debug/Logger.h"


std::vector<Collider*> CollisionManager::Colliders;


void CollisionManager::AddCollider(Collider *collider) {
    Colliders.push_back(collider);
}

void CollisionManager::RemoveCollider(Collider *collider) {
    Colliders.erase(std::remove(Colliders.begin(), Colliders.end(), collider), Colliders.end());
}

void CollisionManager::update() {

    collisions.clear();
    bool collidedWithSmth = false;


    for(int i = 0; i < Colliders.size() - 1; i++)
    {
        Collider* collider = Colliders[i];


        for (int j = i + 1; j < Colliders.size(); j++)
        {
            Collider* another = Colliders[j];
            if(collider->checkCollision(*another))
            {
                Collision c = Collision(collider, another);

                if(!collider->collided || !another->collided)
                {
                    c.firstType = START;
                    c.secondType = START;
                }
                else
                {
                    c.firstType = REMAIN;
                    c.secondType = REMAIN;
                }

                collisions.push_back(c);
                collider->collided = true;
                another->collided = true;
                collider->collidedWith = another->getOwner();
                another->collidedWith = collider->getOwner();
                collidedWithSmth = true;
            }
            else
            {
				collidedWithSmth = false;
            }
//            else if(collider->collided)
//            {
//                collider->collided = false;
//                collider->getOnCollisionExit()(another->getOwner());
//            }
        }

        if(!collidedWithSmth && collider->collided)
        {
            Collision c(collider, NULL, EXIT, EXIT);
            collider->collided = false;
            //collider->collidedWith->getCollider()->collidedWith = nullptr;
            collider->collidedWith = nullptr;


            collisions.push_back(c);
        }

    }

    resolveCollisions();
}

void CollisionManager::resolveCollisions() {
    for (Collision c : collisions) {
        if(c.firstType == START)
        {
            c.first->getOnCollisionStart()(c.second->getOwner());
            c.second->getOnCollisionStart()(c.first->getOwner());
        }
        else if (c.firstType == REMAIN)
        {
            c.first->getOnCollision()(c.second->getOwner());
            c.second->getOnCollision()(c.first->getOwner());
        }
        else
        {
           /* c.first->getOnCollisionExit()(c.second->getOwner());
            if(c.second != NULL)
                c.second->getOnCollisionExit()(c.first->getOwner());*/
        }

    }
}
