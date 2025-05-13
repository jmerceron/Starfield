#include "TextOnScreen_Display.h"
#include "conversion.h"

namespace Julien_Fonts {

    TTF_Font* font = nullptr;

    void init_TTF_font()
    {
        // Initialize SDL_ttf
        if (TTF_Init() < 0)
        {
            SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }

        // Load font
        font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);
        if (font == nullptr)
        {
            SDL_Log("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }
    }

    void display_FPS(SDL_Renderer* renderer, float fps, int x, int y)
    {
        // Convert FPS to string
        char fps_text[64];
        sprintf_s(fps_text, "FPS: %.2f", fps);

        // Create surface from text
        SDL_Color text_color = { 255, 255, 255, 255 }; // White color
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, fps_text, text_color);
        if (text_surface == nullptr)
        {
            SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }

        // Create texture from surface
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        if (text_texture == nullptr)
        {
            SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            SDL_FreeSurface(text_surface);
            return;
        }

        // Set rendering space and render to screen
        SDL_Rect render_quad = { x, y, text_surface->w, text_surface->h };
        SDL_RenderCopy(renderer, text_texture, nullptr, &render_quad);

        // Free resources
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);
    }

    void display_debug_float_value(SDL_Renderer* renderer, const char* text, float value, int x, int y)
    {
        // Convert value to string
        char value_text[64];
        char value_string[10];
        Julien_Conversion::convert_to_string(value, value_string);
        sprintf_s(value_text, "%s%s", text, value_string);

        // Create surface from text
        SDL_Color text_color = { 255, 255, 255, 255 }; // White color
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, value_text, text_color);
        if (text_surface == nullptr)
        {
            SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }

        // Create texture from surface
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        if (text_texture == nullptr)
        {
            SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            SDL_FreeSurface(text_surface);
            return;
        }

        // Set rendering space and render to screen
        SDL_Rect render_quad = { x, y, text_surface->w, text_surface->h };
        SDL_RenderCopy(renderer, text_texture, nullptr, &render_quad);

        // Free resources
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);
    }

    void display_debug_text(SDL_Renderer* renderer, const char* text, const char* value, int x, int y)
    {
        // Convert value to string
        char value_text[64];
        sprintf_s(value_text, "%s%s", text, value);

        // Create surface from text
        SDL_Color text_color = { 255, 255, 255, 255 }; // White color
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, value_text, text_color);
        if (text_surface == nullptr)
        {
            SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }

        // Create texture from surface
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        if (text_texture == nullptr)
        {
            SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            SDL_FreeSurface(text_surface);
            return;
        }

        // Set rendering space and render to screen
        SDL_Rect render_quad = { x, y, text_surface->w, text_surface->h };
        SDL_RenderCopy(renderer, text_texture, nullptr, &render_quad);

        // Free resources
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);
    }

} // namespace Julien_Fonts