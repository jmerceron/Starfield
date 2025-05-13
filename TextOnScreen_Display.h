#pragma once

#include "sdl.h"
#include "SDL_ttf.h"

namespace Julien_Fonts {

    extern TTF_Font* font;

    void init_TTF_font();
    void display_FPS(SDL_Renderer* renderer, float fps, int x, int y);
    void display_debug_float_value(SDL_Renderer* renderer, const char* text, float value, int x, int y);
    void display_debug_text(SDL_Renderer* renderer, const char* text, const char* value, int x, int y);

} // namespace Julien_Fonts