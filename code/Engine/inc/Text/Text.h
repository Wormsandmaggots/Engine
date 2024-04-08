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
#include FT_FREETYPE_H

class Text {
public:
    Text(const std::string& fontPath);
    ~Text();
    void fillCharacterMap();
    void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color, float w, float h);

private:
    FT_Library ft;
    FT_Face face;

    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        FT_Pos Advance;    // Offset to advance to next glyph
    };

    std::map<char, Character> Characters;
};


#endif