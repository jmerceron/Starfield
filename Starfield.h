#pragma once

#include "sdl.h"
#include "SDL_render.h"

namespace Julien_Stars {

    void generate_stars_dynamic_resolution(int width, int height);
    void update_stars(int frame_delay, int width, int height);
    void render_stars(SDL_Renderer* renderer);

} // namespace Julien_Stars