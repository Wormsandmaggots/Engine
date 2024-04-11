//
// Created by jakub on 27.03.2024.
//

#ifndef ENGINE_TEXT_H
#define ENGINE_TEXT_H

#include <ft2build.h>
#include <string>
#include <iostream>
#include <map>
#include "Shader.h"
#include "ECS/Component.h"
#include "Renderer/Renderable.h"
#include FT_FREETYPE_H

class Text : public Component, public Renderable {
public:
    Text(const std::string& fontPath);
    ~Text();
    void fillCharacterMap();
    void renderText();
    void setParameters(std::string text, float _x, float _y, float _scale, glm::vec3 _color, float _w, float _h);
    void awake() override;
    void start() override;
    void update() override;
    void onDestroy() override;
    void setParent(Entity *entity) override;
    void setTransform(Transform2 *transform2) override;
    std::string serialize() override;

    void Draw(Shader *errorshader) override;

private:
    FT_Library ft;
    FT_Face face;
    Shader* shader;
    std::string stringText;
    float x,y,scale,w,h;
    glm::vec3 color;
    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        FT_Pos Advance;    // Offset to advance to next glyph
    };

    std::map<char, Character> Characters;

};


#endif