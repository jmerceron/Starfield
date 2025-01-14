


#include <SDL.h>
#include <iostream>

#include "conversion.h"
#include "Plasma_effect.h"


#define PLASMA_WIDTH 480
#define PLASMA_HEIGHT 360


static Uint16 pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0, tpos1, tpos2, tpos3, tpos4;
static int aSin[512];
static SDL_Color colors_plasma[256];

static SDL_Texture* plasma_texture;


void fn_vDemoScene_ColorPalette_Plasma_init(SDL_Renderer* renderer)
{
    int i;
    float rad;

    /*create sin lookup table */
    for (i = 0; i < 512; i++)
    {
        rad = ((float)i * 0.703125) * 0.0174532; /* 360 / 512 * degree to rad, 360 degrees spread over 512 values to be able to use AND 512-1 instead of using modulo 360*/
        aSin[i] = sin(rad) * 1024; /*using fixed point math with 1024 as base*/
    }

    /* create palette */
    for (i = 0; i < 64; ++i)
    {
        colors_plasma[i].r = i << 2;
        colors_plasma[i].g = 255 - ((i << 2) + 1);
        colors_plasma[i + 64].r = 255;
        colors_plasma[i + 64].g = (i << 2) + 1;
        colors_plasma[i + 128].r = 255 - ((i << 2) + 1);
        colors_plasma[i + 128].g = 255 - ((i << 2) + 1);
        colors_plasma[i + 192].g = (i << 2) + 1;
    }

    // create texture to render Plasma effect
    plasma_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, PLASMA_WIDTH, PLASMA_HEIGHT);

}



void fn_vDemoScene_Plasma_Render_SurfaceApproach(SDL_Window* window, SDL_Renderer* renderer, int ix, int iy, int iwidth, int iheight)
{
    /*
Option 1: Use Surfaces Only
Stick to SDL_GetWindowSurface and SDL_UpdateWindowSurface, and avoid the renderer entirely. Modify your code like this:

cpp
Copier le code
SDL_Surface* surface = SDL_GetWindowSurface(window);
if (surface == NULL) {
    std::cout << "Surface could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    return;
}

Uint8* image;
Uint16 i, j;
Uint8 index;
int x;

// Check if the surface needs locking
if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

// Plasma drawing logic
tpos4 = pos4;
tpos3 = pos3;
image = (Uint8*)((Uint8*)surface->pixels + iy * surface->pitch + ix * 4);

for (i = 0; i < iheight; ++i) {
    tpos1 = pos1 + 5;
    tpos2 = pos2 + 3;

    tpos3 &= 511;
    tpos4 &= 511;

    for (j = 0; j < iwidth * 4; ++j) {
        tpos1 &= 511;
        tpos2 &= 511;

        x = aSin[tpos1] + aSin[tpos2] + aSin[tpos3] + aSin[tpos4];
        index = 128 + (x >> 4);
        *image++ = index;

        tpos1 += 5;
        tpos2 += 3;
    }
    image = (Uint8*)((Uint8*)surface->pixels + (iy + i) * surface->pitch + ix * 4);
    tpos4 += 3;
    tpos3 += 1;
}

// Move plasma
pos1 += 9;
pos3 += 8;

// Unlock and update surface
if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
SDL_UpdateWindowSurface(window);

// Delay for frame rate
SDL_Delay(frameDelay);
    
    */

    SDL_Surface* surface = SDL_GetWindowSurface(window);
    if (surface == NULL)
    {
        std::cout << "Surface could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    // clear screen
    // Uint32 white = SDL_MapRGB(surface->format, 255, 255, 255); // Map white to the surface's format
    // Uint32 random = SDL_MapRGB(surface->format, (std::rand() % 256), (std::rand() % 256), (std::rand() % 256));
    Uint32 red = SDL_MapRGB(surface->format, 128, 0, 0);
    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
    // Get a pointer to the pixel buffer
    Uint8* pixels = (Uint8*)surface->pixels;
    for (int py = 0; py < surface->h; py++) {
        for (int px = 0; px < surface->w; px++) {
            // Calculate the pixel position
            Uint32* pixel = (Uint32*)(pixels + py * surface->pitch + px * surface->format->BytesPerPixel);
            *pixel = red;
        }
    }
    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

    // DO NOT UPDATE HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //    SDL_UpdateWindowSurface(window);


    Uint8* image;
    Uint16 i, j;
    Uint8 index;
    int x;

    /* Lock the screen for direct access to the pixels */
    // Check if the surface needs locking
    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    /* draw plasma */
    tpos4 = pos4;
    tpos3 = pos3;

    image = (Uint8*)((Uint8*)surface->pixels + iy*surface->pitch + ix * 4);

    for (i = 0; i < iheight; ++i)
   {
        tpos1 = pos1 + 5;
        tpos2 = pos2 + 3;

        tpos3 &= 511;
        tpos4 &= 511;

        for (j = 0; j < iwidth*4; ++j)
        {
            tpos1 &= 511;
            tpos2 &= 511;

            x = aSin[tpos1] + aSin[tpos2] + aSin[tpos3] + aSin[tpos4]; /*actual plasma calculation*/

            index = 128 + (x >> 4); /*fixed point multiplication but optimized so basically it says (x * (64 * 1024) / (1024 * 1024)), x is already multiplied by 1024*/

            *image++ = index;
            
            // display ?

            tpos1 += 5;
            tpos2 += 3;
        }
        image = (Uint8*)((Uint8*)surface->pixels + (iy+i) * surface->pitch + ix * 4);

        tpos4 += 3;
        tpos3 += 1;    
    }

    /* move plasma */

    pos1 += 9;
    pos3 += 8;

    // Unlock and update surface
    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
    SDL_UpdateWindowSurface(window);

}

void fn_vDemoScene_Plasma_Render_RendererApproach(SDL_Window* window, SDL_Renderer* renderer, int ix, int iy, int iwidth, int iheight)
{
/*
Option 2: Use the Renderer Only
If you want to use the renderer, you'll need to use a texture instead of directly working with SDL_GetWindowSurface. HereÅfs how:

Create an SDL_Texture to represent your surface:

cpp
Copier le code
SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
Update the texture with the pixel data:

cpp
Copier le code
void* pixels;
int pitch;
SDL_LockTexture(texture, NULL, &pixels, &pitch);

Uint8* image = (Uint8*)pixels;

// Plasma drawing logic
tpos4 = pos4;
tpos3 = pos3;

for (int i = 0; i < iheight; ++i) {
    tpos1 = pos1 + 5;
    tpos2 = pos2 + 3;

    tpos3 &= 511;
    tpos4 &= 511;

    for (int j = 0; j < iwidth * 4; ++j) {
        tpos1 &= 511;
        tpos2 &= 511;

        x = aSin[tpos1] + aSin[tpos2] + aSin[tpos3] + aSin[tpos4];
        index = 128 + (x >> 4);
        *image++ = index;

        tpos1 += 5;
        tpos2 += 3;
    }
    image = (Uint8*)((Uint8*)pixels + (iy + i) * pitch);
    tpos4 += 3;
    tpos3 += 1;
}

SDL_UnlockTexture(texture);
Render the texture:

cpp
Copier le code
SDL_RenderClear(renderer);
SDL_RenderCopy(renderer, texture, NULL, NULL);
SDL_RenderPresent(renderer);
SDL_Delay(frameDelay);
*/


    void* pixels;
    int pitch;
    Uint8 index;
    int x;


    SDL_LockTexture(plasma_texture, NULL, &pixels, &pitch);

    Uint8* image;
    image = (Uint8*)pixels;
//    image = (Uint8*)((Uint8*)pixels + iy * pitch + ix * 4);

    /* draw plasma */
    tpos4 = pos4;
    tpos3 = pos3;

//    for (int i = 0; i < iheight; ++i)
    for (int i = 0; i < PLASMA_HEIGHT; ++i)
    {
        tpos1 = pos1 + 5;
        tpos2 = pos2 + 3;

        tpos3 &= 511;
        tpos4 &= 511;

//        for (int j = 0; j < iwidth * 4; ++j)
        for (int j = 0; j < PLASMA_WIDTH * 4; ++j)
//        for (int j = 0; j < PLASMA_WIDTH; ++j)
        {
            tpos1 &= 511;
            tpos2 &= 511;

            x = aSin[tpos1] + aSin[tpos2] + aSin[tpos3] + aSin[tpos4];
            index = 128 + (x >> 4);
            *image++ = index;

            tpos1 += 5;
            tpos2 += 3;
        }
//        image = (Uint8*)((Uint8*)pixels + (iy + i) * pitch);
        tpos4 += 3;
        tpos3 += 1;
    }

    // unlock texture, we finished updating it
    SDL_UnlockTexture(plasma_texture);

    /* move plasma */

    pos1 += 9;
    pos3 += 8;


    // Define the destination rectangle on the screen
    SDL_Rect destRect;
    destRect.x = ix;
    destRect.y = iy;
    destRect.w = iwidth;
    destRect.h = iheight;

    // Render the texture to the specified rectangle
//    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, plasma_texture, NULL, &destRect);
//    SDL_RenderPresent(renderer);

}



void fn_vDemoScene_Plasma_RenderWithPalette(SDL_Window* window, SDL_Renderer* renderer, int ix, int iy, int iwidth, int iheight)
{

    void* pixels;
    int pitch;
    Uint8 index;
    int x;


    SDL_LockTexture(plasma_texture, NULL, &pixels, &pitch);

    // Convert SDL_PIXELFORMAT_RGBA8888 to SDL_PixelFormat*
    SDL_PixelFormat* format = ConvertToPixelFormat(SDL_PIXELFORMAT_RGBA8888);

    Uint32* image;
    image = (Uint32*)pixels;

    /* draw plasma */
    tpos4 = pos4;
    tpos3 = pos3;

    for (int i = 0; i < PLASMA_HEIGHT; i++)
    {
        tpos1 = pos1 + 5;
        tpos2 = pos2 + 3;

        tpos3 &= 511;
        tpos4 &= 511;

        for (int j = 0; j < PLASMA_WIDTH; j++)
        {
            tpos1 &= 511;
            tpos2 &= 511;

            x = aSin[tpos1] + aSin[tpos2] + aSin[tpos3] + aSin[tpos4];
            index = 128 + (x >> 4);
//            *image++ = index;
//            *image++ = SDL_ColorToRGBA8888(colors_plasma[index], format);
            *image = SDL_ColorToRGBA8888(colors_plasma[index], format);
            image++;

            tpos1 += 5;
            tpos2 += 3;
        }
        tpos4 += 3;
        tpos3 += 1;
    }

    SDL_UnlockTexture(plasma_texture);

    /* move plasma */

    pos1 += 9;
    pos3 += 8;


    // Define the destination rectangle on the screen
    SDL_Rect destRect;
    destRect.x = ix;
    destRect.y = iy;
    destRect.w = iwidth;
    destRect.h = iheight;

    // Render the texture to the specified rectangle
    SDL_RenderCopy(renderer, plasma_texture, NULL, &destRect);

}
