#pragma once

#include "sdl.h"
#include "SDL_render.h"

namespace Julien_Plasma {

    void init_plasma(SDL_Renderer* renderer);
    void update_plasma_with_palette();
    void render_plasma_with_palette(SDL_Renderer* renderer, int x, int y, int width, int height);
    void render_plasma_surface(SDL_Window* window, SDL_Renderer* renderer, int x, int y, int width, int height);

} // namespace Julien_Plasma