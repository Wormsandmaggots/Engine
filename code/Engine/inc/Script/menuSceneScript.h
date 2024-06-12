#ifndef ENGINE_MENUSCENESCRIPT_H
#define ENGINE_MENUSCENESCRIPT_H

#include "Script/SceneScript.h"

class menuSceneScript: public SceneScript{
public:
    void awake() override{};
    void start() override{};
    void update() override{};
    void onDestroy() override{};
    ~menuSceneScript() override = default;
};

#endif
