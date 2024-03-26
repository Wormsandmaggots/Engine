
#ifndef ENGINE_COMPONENT_H
#define ENGINE_COMPONENT_H

class Component {

    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void OnDestroy() = 0;

};

#endif //ENGINE_COMPONENT_H
