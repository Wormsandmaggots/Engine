#pragma once
#include "glm/vec3.hpp"
#include "glm/detail/type_mat4x4.hpp"

class ScoreNumbers{
public:
    ScoreNumbers(const ScoreNumbers&) = delete;
    ScoreNumbers& operator=(const ScoreNumbers&) = delete;

    static ScoreNumbers& getInstance() {
        static ScoreNumbers instance;
        return instance;
        textRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    }

    void calculateViewPos(glm::vec3 pos){
        glm::vec4 orbClipSpace = projection * view * glm::vec4(pos, 1.0f);
        glm::vec3 orbNDC = glm::vec3(orbClipSpace) / orbClipSpace.w;
        glm::vec2 orbScreenPos;
        orbScreenPos.x = (orbNDC.x * 0.5f + 0.5f) * width;
        orbScreenPos.y = (orbNDC.y * 0.5f + 0.5f) * height;
        orbScreenPos.y = height - orbScreenPos.y;
        LOG_INFO(std::to_string(orbScreenPos.x) + " " + std::to_string(orbScreenPos.y));
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
    ScoreNumbers() {}
    ~ScoreNumbers() {}
    glm::mat4 projection;
    glm::mat4 view;
    int width;
    int height;
    static Text *textRenderer;
};