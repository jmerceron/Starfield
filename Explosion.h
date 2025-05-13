#pragma once

#include "sdl.h"
#include "SDL_render.h"

namespace Julien_Explosion {

    void init_explosion(SDL_Renderer* renderer);
    void init_particles_randomly();
    void update_explosion_xy();
    void render_explosion_xy(SDL_Renderer* renderer, int x, int y, int width, int height);
    void render_explosion_xy_fast_ugly(SDL_Renderer* renderer, int x, int y, int width, int height);
    void render_explosion(SDL_Renderer* renderer);

} // namespace Julien_Explosion