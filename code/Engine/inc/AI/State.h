#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H
#define GLM_ENABLE_EXPERIMENTAL
#include "ECS/Entity.h"
#include "Core/Time.h"
#include "Animation/Animator.h"
class State {
public:
    Animator* animator;
    Animation* givenAnimation;
    State() {}
    State(Animation* anim) {
        givenAnimation = anim;
    }

    virtual void enter(Entity* contex) {
        if(givenAnimation == NULL){
            givenAnimation = new Animation(contex->getComponent<Model>()->getPath(), contex->getComponent<Model>());
        }
        //Animation* animation = new Animation(contex->getComponent<Model>()->getPath(), contex->getComponent<Model>());
        animator = new Animator(givenAnimation);
    };
    virtual void update(Entity* context) {
        context->getComponent<Model>()->getShader()->use();
        animator->UpdateAnimation(Time::deltaTime);
        auto transforms = animator->GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            context->getComponent<Model>()->getShader()->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


    };
    virtual void exit(Entity* context) {};

};


#endif //ENGINE_STATE_H
