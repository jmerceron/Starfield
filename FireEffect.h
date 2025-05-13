#pragma once

#include "sdl.h"
#include "SDL_render.h"

namespace Julien_Fire {

    void init_fire_palette();
    void render_fire_adaptive_resolution(SDL_Renderer* renderer, int width, int height);

} // namespace Julien_Fire