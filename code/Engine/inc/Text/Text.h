//
// Created by jakub on 27.03.2024.
//

#ifndef ENGINE_TEXT_H
#define ENGINE_TEXT_H

#include <ft2build.h>
#include <string>
#include <iostream>
#include FT_FREETYPE_H

class Text {
public:
    Text(const std::string& fontPath);

    ~Text();

    void RenderText(const std::string& text, float x, float y, float scale);

private:
    FT_Library ft;
    FT_Face face;
};


#endif