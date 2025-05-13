#pragma once

#include "sdl.h"
#include "SDL_render.h"

namespace Julien_FullScreenFX {

    struct MotionBlur 
    {
        // Motion Blur declarations
        SDL_Texture* screen_capture_texture;
        SDL_Texture* blur_accumulation_texture;
        float blur_strength;            // Adjust this (0.0 = no blur, 0.95 = heavy blur)
        int blur_passes;                // More passes = stronger blur
        bool is_motion_blur_enabled;    // Toggle with a key if desired
    };

    std::string motion_blur_toggle_state();
    void motion_blur_toggle_on_off();
    float motion_blur_strength_value();       
    void motion_blur_strengthen();
    void motion_blur_weaken();
    void init_motion_blur(SDL_Renderer* renderer, int width, int height);
    void update_motion_blur(SDL_Renderer* renderer, int width, int height);
    void set_render_target_if_motion_blur_is_activated(SDL_Renderer* renderer);
    void set_debug_render_target_if_motion_blur_is_activated(SDL_Renderer* renderer);
    void render_motion_blur(SDL_Renderer* renderer);
    void destroy_motion_blur();


    void debug_draw_screen_capture(SDL_Renderer* renderer);
    void debug_draw_accumulation(SDL_Renderer* renderer);


} // namespace Julien_FullScreenFX
