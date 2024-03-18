#ifndef ENGINE_COMPONENT_H
#define ENGINE_COMPONENT_H

class Component {
public:
    virtual void Start() = 0;
    virtual void Upadte() = 0;
};

#endif //ENGINE_COMPONENT_H
