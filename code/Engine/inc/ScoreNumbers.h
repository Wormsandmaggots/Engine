#ifndef ENGINE_SCORENUMBERS_H
#define ENGINE_SCORENUMBERS_H
#include "glm/vec3.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "Text/Text.h"
#include <vector>
#include <iostream>

class ScoreNumbers{
public:
    struct Points {
        int x, y, p, t;
        Points(int x, int y, int p, int t) : x(x), y(y), p(p), t(t) {}
    };

    ScoreNumbers(const ScoreNumbers&) = delete;
    ScoreNumbers& operator=(const ScoreNumbers&) = delete;

    static ScoreNumbers& getInstance() {
        static ScoreNumbers instance;
        return instance;
    }

    void calculateViewPos(glm::vec3 pos, int points){
        glm::vec4 orbClipSpace = projection * view * glm::vec4(pos, 1.0f);
        glm::vec3 orbNDC = glm::vec3(orbClipSpace)/orbClipSpace.w;
        glm::vec2 orbScreenPos;
        orbScreenPos.x = (orbNDC.x * 0.5f + 0.5f) * width;
        orbScreenPos.y = (orbNDC.y * 0.5f + 0.5f) * height;
        //orbScreenPos.y = height - orbScreenPos.y;
        pointsStorage.push_back(Points(orbScreenPos.x, orbScreenPos.y, points, 1));
        LOG_INFO(std::to_string(orbScreenPos.x) + " " +std::to_string(orbScreenPos.y));
    }

    void update(){
        for (auto it = pointsStorage.begin(); it != pointsStorage.end(); ) {
            textRenderer->setParameters(std::to_string(it->p), it->x, it->y, 1.2f/it->t, glm::vec3(0.5, 0.8f, 0.2f), width, height);
            if (it->t < 50) {
                it->t += 1;
                ++it;
                textRenderer->renderText();
            } else {
                it = pointsStorage.erase(it);
            }
        }

    }

    void setProjection(glm::mat4 p){
        projection = p;
    }
    void setView(glm::mat4 v){
        view = v;
    }
    void setWidth(int w){
        width = w;
    }
    void setHeight(int h){
        height = h;
    }


private:
    ScoreNumbers() {
        textRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    }
    ~ScoreNumbers() {}
    glm::mat4 projection;
    glm::mat4 view;
    int width;
    int height;
    Text *textRenderer;
    std::vector<Points> pointsStorage;
};

#endif