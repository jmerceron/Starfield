#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

#include "TextOnScreen_Display.h"



SDL_Texture* pstMyFPS_Texture;
SDL_Texture* pstMyDebugText_Texture;
SDL_Texture* pstMyInfoText_Texture;




bool fn_bInit_TTF_Font() 
{
    pstFont = nullptr;

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    pstFont = TTF_OpenFont("assets/fonts/Lato-Regular.ttf", 24); // Use a valid font path
    if (!pstFont) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}


void fn_vDisplayFPS(SDL_Renderer* renderer, float fps)
{
    // Convert FPS to a string
    std::string fpsText = "FPS: " + std::to_string(static_cast<int>(fps));

    // Render the text to a surface
    SDL_Color textColor = { 255, 255, 255, 255 }; // White
    SDL_Surface* textSurface = TTF_RenderText_Solid(pstFont, fpsText.c_str(), textColor);
    if (!textSurface) 
    {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Create a texture from the surface
    pstMyFPS_Texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!pstMyFPS_Texture) {
        std::cerr << "Unable to create texture from rendered text! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Get the width and height of the text
    int textWidth, textHeight;
    SDL_QueryTexture(pstMyFPS_Texture, NULL, NULL, &textWidth, &textHeight);

    // Define the destination rectangle on the screen
    SDL_Rect renderQuad = { 10, 10, textWidth, textHeight };

    // Render the texture
    SDL_RenderCopy(renderer, pstMyFPS_Texture, NULL, &renderQuad);

    // Clean up
    SDL_DestroyTexture(pstMyFPS_Texture);
}


