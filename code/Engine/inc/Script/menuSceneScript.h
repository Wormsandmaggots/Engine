#ifndef ENGINE_MENUSCENESCRIPT_H
#define ENGINE_MENUSCENESCRIPT_H

#include "Script/SceneScript.h"

class menuSceneScript: public SceneScript{
public:
    void awake() override{};
    void start() override{};
    void update(const glm::mat4& projection, const glm::mat4& view) override{};
    void onDestroy() override{};
    ~menuSceneScript() override = default;
};

#endif
