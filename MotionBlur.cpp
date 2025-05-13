#include <string>

#include "MotionBlur.h"


namespace Julien_FullScreenFX {

    MotionBlur blur;

    std::string motion_blur_toggle_state()
    {
        if (blur.is_motion_blur_enabled)
            return "ON";
        else
            return "OFF";
    }

    void motion_blur_toggle_on_off()
    {
        blur.is_motion_blur_enabled = 1 - blur.is_motion_blur_enabled;
    }

    float motion_blur_strength_value()
    {
        return blur.blur_strength;
    }

    void motion_blur_strengthen()
    {
        if (blur.blur_strength < 0.99f)
            blur.blur_strength += 0.01f;
    }

    void motion_blur_weaken()
    {
        if (blur.blur_strength > 0.01f)
            blur.blur_strength -= 0.01f;
    }

    void init_motion_blur(SDL_Renderer* renderer, int width, int height)
    {

        blur.is_motion_blur_enabled = true;
        blur.blur_strength = 0.85f; // e.g., 0.85f
        blur.blur_passes = 3;

        // Create blur texture (match window size)
        blur.screen_capture_texture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            width, height);
        if (!blur.screen_capture_texture) {
            SDL_Log("Failed to create current frame texture: %s", SDL_GetError());
            return;
        }        
        
        // Create blur texture (match window size)
        blur.blur_accumulation_texture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            width, height);
        if (!blur.blur_accumulation_texture) {
            SDL_Log("Failed to create current frame texture: %s", SDL_GetError());
            return;
        }

        // Clear blur_accumulation_texture
        SDL_SetRenderTarget(renderer, blur.blur_accumulation_texture);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, NULL);

        // Clear screen_capture_texture
        SDL_SetRenderTarget(renderer, blur.screen_capture_texture);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, NULL);


        // Set blend mode to ensure proper alpha blending
        if (SDL_SetTextureBlendMode(blur.screen_capture_texture, SDL_BLENDMODE_NONE) < 0) {
        //if (SDL_SetTextureBlendMode(blur.screen_capture_texture, SDL_BLENDMODE_BLEND) < 0) {
            SDL_Log("Failed to set blend mode for current frame: %s", SDL_GetError());
        }
        // Set blend mode to ensure proper alpha blending
        if (SDL_SetTextureBlendMode(blur.blur_accumulation_texture, SDL_BLENDMODE_BLEND) < 0) {
            SDL_Log("Failed to set blend mode for current frame: %s", SDL_GetError());
        }

    }

    void update_motion_blur(SDL_Renderer* renderer, int width, int height) 
    {
        destroy_motion_blur();
        init_motion_blur(renderer, width, height);
    }

    void set_render_target_if_motion_blur_is_activated(SDL_Renderer* renderer)
    {
        if (blur.is_motion_blur_enabled) {
            // Step 1: Render the scene to render target texture
            SDL_SetRenderTarget(renderer, blur.screen_capture_texture);
        }
        else {
            // Temporarily reset render target to NULL to update plasma_texture and explosion_texture
            SDL_SetRenderTarget(renderer, NULL);
        }
    }

    void set_debug_render_target_if_motion_blur_is_activated(SDL_Renderer* renderer)
    {
        if (blur.is_motion_blur_enabled) {
            // Step 1: Render the scene to render target texture
            SDL_SetRenderTarget(renderer, blur.blur_accumulation_texture);
        }
        else {
            // Temporarily reset render target to NULL to update plasma_texture and explosion_texture
            SDL_SetRenderTarget(renderer, NULL);
        }
    }

    void render_motion_blur(SDL_Renderer* renderer)
    {

        if (blur.is_motion_blur_enabled) 
        {
            // Step 1: Copy screen_capture_texture to backBuffer
            SDL_SetRenderTarget(renderer, nullptr);
            SDL_SetTextureAlphaMod(blur.screen_capture_texture, 255); // Current frame contribution 1-alpha
            SDL_RenderCopy(renderer, blur.screen_capture_texture, NULL, NULL);

            // Step 2: Blend blur_texture with accumulation_texture
            SDL_SetRenderTarget(renderer, blur.blur_accumulation_texture);
            SDL_SetTextureAlphaMod(blur.blur_accumulation_texture, static_cast<Uint8>((blur.blur_strength) * 255)); // Current frame contribution 1-alpha
            SDL_SetTextureBlendMode(blur.screen_capture_texture, SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(blur.screen_capture_texture, static_cast<Uint8>((1.0f - blur.blur_strength) * 255)); // Current frame contribution 1-alpha
            SDL_RenderCopy(renderer, blur.screen_capture_texture, NULL, NULL);
            SDL_SetTextureBlendMode(blur.screen_capture_texture, SDL_BLENDMODE_NONE);

            // Step 3: Render the accumulation_texture to the screen
            SDL_SetRenderTarget(renderer, NULL); // Reset to default render target (screen)
            SDL_SetTextureAlphaMod(blur.blur_accumulation_texture, 255); // Current frame contribution
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            SDL_RenderCopy(renderer, blur.blur_accumulation_texture, NULL, NULL);
    
        }

        // 4. Render UI elements (on top of everything)
        if (SDL_SetRenderTarget(renderer, nullptr)!=0) {
            SDL_Log("Failed to fallback to No Render Target Mode: %s", SDL_GetError());
        }
    }


    void debug_draw_screen_capture(SDL_Renderer* renderer)
    {
        SDL_RenderCopy(renderer, blur.screen_capture_texture, NULL, NULL);
    }
    void debug_draw_accumulation(SDL_Renderer* renderer)
    {
        SDL_RenderCopy(renderer, blur.blur_accumulation_texture, NULL, NULL);
    }

    void destroy_motion_blur() 
    {
        if (blur.screen_capture_texture)
        {
            SDL_DestroyTexture(blur.screen_capture_texture);
            blur.screen_capture_texture = nullptr;
        }
        if (blur.blur_accumulation_texture)
        {
            SDL_DestroyTexture(blur.blur_accumulation_texture);
            blur.blur_accumulation_texture = nullptr;
        }
    }

} // namespace Julien_FullScreenFX