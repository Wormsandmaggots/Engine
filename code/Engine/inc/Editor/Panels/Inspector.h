//
// Created by Radek on 06.04.2024.
//

#ifndef ENGINE_INSPECTOR_H
#define ENGINE_INSPECTOR_H


#include "ECS/Entity.h"
#include "Editor/Camera.h"

class Inspector {
public:
    static void draw(Entity*, Camera*);
};


#endif //ENGINE_INSPECTOR_H
