
#ifndef ENGINE_TRANSFORM_H
#define ENGINE_TRANSFORM_H
#include "Component.h"
#include <vector>
class Transform: public Component{
public:
    std::vector<Transform> children;
};

#endif //ENGINE_TRANSFORM_H
