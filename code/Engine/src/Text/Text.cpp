#include "Text/Text.h"

Text::Text(const std::string& fontPath) {
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
}

Text::~Text() {
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Text::RenderText(const std::string& text, float x, float y, float scale) {
    for (char c : text) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            continue;
        }
        //redering of symbols
    }
}