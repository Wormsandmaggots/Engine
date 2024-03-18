
#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H
#include <vector>
class Component;

class Entity{
    public:
    std::vector<Component> componentList;

};

#endif //ENGINE_ENTITY_H
