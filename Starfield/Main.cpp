
// SpaceInvaders_Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Julien Merceron - 2023
// code generated by chatGPT
/*
ChatGPT Dec 15th 2022 Version
https://chat.openai.com/chat
*/
/*
SDL
https://github.com/libsdl-org/SDL/releases/tag/release-2.26.2
SDL2-devel-2.26.2-VC.zip
sdl/include
sdl/lib
wrt dlls, ...
... copy \SDL2\lib\x64\SDL2.dll to C:\Windows\System32
... and \SDL2\lib\x86\SDL2.dll to C:\Windows\SysWOW64. Voila
*/
/*
SDL Image
https://github.com/libsdl-org/SDL_image
TBC
*/
/*
create a CONSOLE APP on VC++
copy paste base code in c++ file
------
add include path to SDL in VC++ Directories
add lib path to SDL in VC++ Directories
add sdl2.lib in libs to link in Linker -> Input
change from console to windows in Linker -> System
*/
/*
PROMPT MODIFIERS
resolution_driven
add_comments
*/



#include "sdl.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include <iostream>
#include <windows.h>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <cstdint>
#include <omp.h>

#define STARFIELD_DEBUG 0
#define HELLO_WORLD 0
#define PLASMA_SURFACE_FX 0 /* it crashes when resolution goes back from FULL SCREEN - check i,j loop */
#define PLASMA_RENDERER_FX 0
#define EXPLOSION_FX 0
#define FIRE_FX 0
#define STARFIELD_FX 0
#define MAIN_DEMO 100000 /* Invaders destruction is correlated + timing to double check */


// Screen dimensions (as percentages of the actual screen dimensions)
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360
const int SCREEN_WIDTH_PERCENT = 80;
const int SCREEN_HEIGHT_PERCENT = 80;
int screen_width, screen_height;
int game_screen_width;
int game_screen_height;


#include "SDL_Controller.h"
#include "Player_Actions.h"

#include "sdl_Render_Circle.h"
#include "player_bullets_invaders.h"

#include "starfield.h"

#include "FireEffect.h"
#include "Plasma_effect.h"

#include "Explosion.h"

#include "TextOnScreen_Display.h"


// this is the MAIN function

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) < 0)
    {
        if (STARFIELD_DEBUG) std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // detect and enable controllers
    SDL_GameController* controller = nullptr;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        controller = openController(i);
        if (controller) {
            break; // Assume we only care about the first controller for this example
        }
    }
    if (STARFIELD_DEBUG && !controller)
    {
        std::cerr << "No compatible controller found!" << std::endl;
    }


    // Flag to track whether the window is in full screen mode
    bool fullScreen = false;

    // Get the actual screen dimensions
    SDL_DisplayMode displayMode;

    // SDL_GetCurrentDisplayMode(0, &screen_width, &screen_height);
    if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0)
    {
        // Success! The current display mode is stored in the "displayMode" variable
        screen_width = displayMode.w;
        screen_height = displayMode.h;
        if (STARFIELD_DEBUG) std::cout << "Success! The current display mode is stored in the displayMode variable, and we have " << screen_width << ", " << screen_height << std::endl;
    }
    else
    {
        // An error occurred. You can use SDL_GetError() to get the error message.
        if (STARFIELD_DEBUG) std::cout << "An error occurred. You can use SDL_GetError() to get the error message" << std::endl;
        return -1;
    }

    // Calculate the dimensions of the game screen (as pixels)
    game_screen_width = screen_width * SCREEN_WIDTH_PERCENT / 100;
    game_screen_height = screen_height * SCREEN_HEIGHT_PERCENT / 100;

    // Create the window
    SDL_Window* window = SDL_CreateWindow("Starfield", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, game_screen_width, game_screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        if (STARFIELD_DEBUG) std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create the renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        if (STARFIELD_DEBUG) std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    fn_bInit_TTF_Font();

    // Set the frame rate
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    // set the counters
    Uint64 iStartTime, iEndTime;
    float fEstimated_Fps = 0.0f;

    // loop variable 
    bool running = true;
    // shoot variable
    bool bShoot = 0;
    // Player action mapping
    int	 iPlayer_action = 0;


#if HELLO_WORLD
    // Set the window title
    SDL_SetWindowTitle(window, "Julien Hello World Demo");

    std::cout << "Hello World!\n";
    return 0;

#elif PLASMA_SURFACE_FX
    // Set the window title
    SDL_SetWindowTitle(window, "Julien Plasma [SURFACE] Effect Demo");

    // Init Game Objects
    fn_vInitObjectsSizeOnScreen();

    // Plasma Color Palette
    fn_vDemoScene_ColorPalette_Plasma_init();

    // Game loop
    while (running)
    {

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if ((event.type == SDL_QUIT) || (event.key.keysym.sym == SDLK_ESCAPE))
            {
                running = false;
            }
            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    // Handle window resized event
                    game_screen_width = event.window.data1;
                    game_screen_height = event.window.data2;
                    fn_vUpdateObjectsSizeOnScreen();
                }
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f)
            {
                // Toggle full screen mode
                fullScreen = !fullScreen;
                SDL_SetWindowFullscreen(window, fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    mPlayer.velocity = -5;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    mPlayer.velocity = 5;
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT)
                {
                    mPlayer.velocity = 0;
                }
            }
        }

        // Update game state
        mPlayer.update(game_screen_width);

        // plasma effect (which includes clear screen)
        // we should do it on vessels !!!!!
        fn_vDemoScene_Plasma_Render_SurfaceApproach(window, renderer, mPlayer.x, mPlayer.y, mPlayer.width, mPlayer.height);

        // Delay to achieve desired frame rate
        SDL_Delay(frameDelay);

    }

#elif PLASMA_RENDERER_FX
    // Set the window title
    SDL_SetWindowTitle(window, "Julien Plasma [RENDERER] Effect Demo");

    // Init Game Objects
    fn_vInitObjectsSizeOnScreen();

    // Plasma Color Palette
    fn_vDemoScene_ColorPalette_Plasma_init(renderer);

    // clear color increment
    int k = 0;

    // Game loop
    while (running)
    {

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if ((event.type == SDL_QUIT) || (event.key.keysym.sym == SDLK_ESCAPE))
            {
                running = false;
            }
            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    // Handle window resized event
                    game_screen_width = event.window.data1;
                    game_screen_height = event.window.data2;
                    fn_vUpdateObjectsSizeOnScreen();
                }
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f)
            {
                // Toggle full screen mode
                fullScreen = !fullScreen;
                SDL_SetWindowFullscreen(window, fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    mPlayer.velocity = -5;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    mPlayer.velocity = 5;
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT)
                {
                    mPlayer.velocity = 0;
                }
            }
        }

        // Update game state
        mPlayer.update(game_screen_width);


        // clear background
        // Set the rendering color
        SDL_SetRenderDrawColor(renderer, k, 0, 0, 255);
        k += 1;
        if (k > 255) k = 0;
        // Clear the window
        SDL_RenderClear(renderer);


        // plasma effect (which includes clear screen)
        // we should do it on vessels !!!!!
        fn_vDemoScene_Plasma_RenderWithPalette(window, renderer, mPlayer.x, mPlayer.y, mPlayer.width, mPlayer.height);

        // Display the window
        SDL_RenderPresent(renderer);

        // Delay to achieve desired frame rate
        SDL_Delay(frameDelay);

    }

#elif EXPLOSION_FX
    // Set the window title
    SDL_SetWindowTitle(window, "Julien Explosion Effect Demo");


    // init Color palette and particles buffer
    fn_vInit_Explosion(renderer);

    // clear color increment
    int k = 0;

    // Game loop
    while (running)
    {

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if ((event.type == SDL_QUIT) || (event.key.keysym.sym == SDLK_ESCAPE))
            {
                running = false;
            }
            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    // Handle window resized event
                    game_screen_width = event.window.data1;
                    game_screen_height = event.window.data2;
                }
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f)
            {
                // Toggle full screen mode
                fullScreen = !fullScreen;
                SDL_SetWindowFullscreen(window, fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_SPACE) // trigger new explosion !!!!!!!
                {
                    // re-init particles (not the color palette)
                    fn_vRandomly_Init_Particles();
                    // we could reinit here thew color palette to make more color variety...
                }
            }
        }


        // clear background
         // Set the rendering color
        SDL_SetRenderDrawColor(renderer, k, 0, 0, 255);
        k += 1;
        if (k > 255) k = 0;
        // Clear the window
        SDL_RenderClear(renderer);


        // explosion effect
        // we should do it on enemies !!!!!
        fm_iRun_Explosion_RendererStyle(renderer);

        // Display the window
        SDL_RenderPresent(renderer);

        // Delay to achieve desired frame rate
        SDL_Delay(frameDelay);

    }

#elif FIRE_FX
    // Set the window title
    SDL_SetWindowTitle(window, "Julien Fire Effect Demo");

    // Fire Color Palette
    fn_vDemoScene_ColorPalette_Fire_init();

    // Game loop
    while (running)
    {

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if ((event.type == SDL_QUIT) || (event.key.keysym.sym == SDLK_ESCAPE))
            {
                running = false;
            }
            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    // Handle window resized event
                    game_screen_width = event.window.data1;
                    game_screen_height = event.window.data2;
                }
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f)
            {
                // Toggle full screen mode
                fullScreen = !fullScreen;
                SDL_SetWindowFullscreen(window, fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            }
        }


        // Set the rendering color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Clear the window
        SDL_RenderClear(renderer);

        // fire effect
        fn_vDemoScene_Fire_Render(renderer);

        // Display the window
        SDL_RenderPresent(renderer);

        // Delay to achieve desired frame rate
        SDL_Delay(frameDelay);

    }

#elif STARFIELD_FX
    // Set the window title
    SDL_SetWindowTitle(window, "Julien Starfield Effect Demo");

    // starfield
    generateStars();
    // Game loop
    while (running)
    {

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if ((event.type == SDL_QUIT) || (event.key.keysym.sym == SDLK_ESCAPE))
            {
                running = false;
            }
            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    // Handle window resized event
                    game_screen_width = event.window.data1;
                    game_screen_height = event.window.data2;
                    // Do something with newWidth and newHeight
                    generateStars();
                }
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f)
            {
                // Toggle full screen mode
                fullScreen = !fullScreen;
                SDL_SetWindowFullscreen(window, fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            }
        }

        // update the starfield
        updateStars(frameDelay);

        // Set the rendering color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Clear the window
        SDL_RenderClear(renderer);

        // render starfield
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (const Star& star : stars)
        {
            //            SDL_RenderDrawPoint(renderer, star.x, star.y);
            SDL_Rect starRect = { star.x, star.y, 3, 3 };
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &starRect);
        }

        // Display the window
        SDL_RenderPresent(renderer);

        // Delay to achieve desired frame rate
        SDL_Delay(frameDelay);

    }

#else
    // Set the window title
    SDL_SetWindowTitle(window, "Julien's Demo");

    // init the destruction system that triggers explosions
    fn_vDestruction_Init();

    // Init Game Objects
    fn_vInitObjectsSizeOnScreen();

    // starfield
    generateStars();

    // Fire Color Palette
    fn_vDemoScene_ColorPalette_Fire_init();

    // Plasma Color Palette
    fn_vDemoScene_ColorPalette_Plasma_init(renderer);

    // init explosion
    fn_vInit_Explosion(renderer);

    // Game loop
    while (running)
    {

        // capture FPS data here
        iStartTime = SDL_GetPerformanceCounter();

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if ((event.type == SDL_QUIT)||(event.key.keysym.sym == SDLK_ESCAPE))
            {
                running = false;
            }
            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    // Handle window resized event
                    game_screen_width = event.window.data1;
                    game_screen_height = event.window.data2;
                    // Do something with newWidth and newHeight
                    fn_vUpdateObjectsSizeOnScreen();
                    generateStars();
                }
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f)
            {
                // Toggle full screen mode
                fullScreen = !fullScreen;
                SDL_SetWindowFullscreen(window, fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    mPlayer.velocity = -5;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    mPlayer.velocity = 5;
                }
                else if (event.key.keysym.sym == SDLK_SPACE) // bolean shoot !!!!!!!
                {
                    if (!bShoot)
                    {
                        bShoot = 1;
                        fn_vUpdateBullets_spawn5();
                    }
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT)
                {
                    mPlayer.velocity = 0;
                }
                else if (event.key.keysym.sym == SDLK_SPACE) // bolean shoot !!!!!!!
                {
                    if (bShoot)
                    {
                        bShoot = 0;
                    }
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                // Handle mouse input
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY); // Get the mouse position
                fn_vUpdateBullets_spawn1(mouseX, mouseY);
            }
            else 
            {
                // Handle button presses
                if (event.type == SDL_CONTROLLERBUTTONDOWN) 
                {
                    const char* action = (event.type == SDL_CONTROLLERBUTTONDOWN) ? "pressed" : "released";

                    // Check specific buttons
                    SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(event.cbutton.button);
                    if (button == SDL_CONTROLLER_BUTTON_Y)
                    {
                        // Triangle button
                        if (STARFIELD_DEBUG) std::cout << "Triangle button " << action << std::endl;
                        if (!bShoot)
                        {
                            bShoot = 1;
                            fn_vUpdateBullets_spawn5();
                        }
                    }
                    else if (button == SDL_CONTROLLER_BUTTON_X)
                    {
                        // Square button
                        if (STARFIELD_DEBUG) std::cout << "Square button " << action << std::endl;
                        if (!bShoot)
                        {
                            bShoot = 1;
                            int iLocationX = mPlayer.x + mPlayer.width/2;
                            int iLocationY = mPlayer.y;
                            fn_vUpdateBullets_spawn1(iLocationX, iLocationY);
                        }
                    }
                    else if (button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
                    {
                        // LEFT button
                        if (STARFIELD_DEBUG) std::cout << "D-Pad Left " << action << std::endl;
                        mPlayer.velocity = -5;
                    }
                    else if (button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) 
                    {
                        // RIGHT button
                        if (STARFIELD_DEBUG) std::cout << "D-Pad Right " << action << std::endl;
                        mPlayer.velocity = 5;
                    }
                }
                if (event.type == SDL_CONTROLLERBUTTONUP)
                {
                    const char* action = (event.type == SDL_CONTROLLERBUTTONDOWN) ? "pressed" : "released";

                    // Check specific buttons
                    SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(event.cbutton.button);
                    if (button == SDL_CONTROLLER_BUTTON_Y)
                    {
                        // Triangle button
                        if (STARFIELD_DEBUG) std::cout << "Triangle button " << action << std::endl;
                        if (bShoot)
                        {
                            bShoot = 0;
                        }
                    }
                    else if (button == SDL_CONTROLLER_BUTTON_X)
                    {
                        // Square button
                        if (STARFIELD_DEBUG) std::cout << "Square button " << action << std::endl;
                        if (bShoot)
                        {
                            bShoot = 0;
                        }
                    }
                    else if (button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
                    {
                        // LEFT button
                        if (STARFIELD_DEBUG) std::cout << "D-Pad Left " << action << std::endl;
                        mPlayer.velocity = 0;
                    }
                    else if (button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                    {
                        // RIGHT button
                        if (STARFIELD_DEBUG) std::cout << "D-Pad Right " << action << std::endl;
                        mPlayer.velocity = 0;
                    }

                }

                // Handle axis (stick) movements
                if (event.type == SDL_CONTROLLERAXISMOTION) 
                {
                    SDL_GameControllerAxis axis = static_cast<SDL_GameControllerAxis>(event.caxis.axis);
                    int value = event.caxis.value; // Axis value (-32768 to 32767)

                    // Detect significant movement (ignore small drifts with a deadzone threshold)
                    const int deadzone = 8000; // Adjust as needed
                    if (axis == SDL_CONTROLLER_AXIS_LEFTX) 
                    {
                        if (value < -deadzone) {
                            if (STARFIELD_DEBUG) std::cout << "Left Stick moved left with value: " << value << std::endl;
                            mPlayer.velocity = value/1000;
                        }
                        else if (value > deadzone) {
                            if (STARFIELD_DEBUG) std::cout << "Left Stick moved right with value: " << value << std::endl;
                            mPlayer.velocity = value / 1000;
                        }
                        else
                            mPlayer.velocity = 0;
                    }
                    else if (axis == SDL_CONTROLLER_AXIS_RIGHTX) 
                    {
                        if (value < -deadzone) {
                            if (STARFIELD_DEBUG) std::cout << "Right Stick moved left with value: " << value << std::endl;
                            mPlayer.velocity = value / 1000;
                        }
                        else if (value > deadzone) {
                            if (STARFIELD_DEBUG) std::cout << "Right Stick moved right with value: " << value << std::endl;
                            mPlayer.velocity = value / 1000;
                        }
                        else
                            mPlayer.velocity = 0;
                    }
                }
            }
        }

        // Update game state
        mPlayer.update(game_screen_width);
        fn_vPlayerBullets_Update();

        int iNb_Threads;
       #pragma omp parallel for
        for (int i = 0; i < vInvaders.size(); i++)
        {
            vInvaders[i].update();
        }
        iNb_Threads = omp_get_max_threads();

        // update the starfield
        updateStars(frameDelay);

        // Set the rendering color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Clear the window
        SDL_RenderClear(renderer);
#if SCROLLING_STARFIELD
        // render starfield
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (const Star& star : stars)
        {
            //            SDL_RenderDrawPoint(renderer, star.x, star.y);
            SDL_Rect starRect = { star.x, star.y, 3, 3 };
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &starRect);
        }
#else if
        // Draw stars
        for (const Star& star : stars)
        {
            // Calculate star size based on depth
            int starSize = static_cast<int>(STAR_SIZE * (MAX_STAR_DEPTH - star.depth + 1));

            // Set star color based on depth
            int color = static_cast<int>((1 - star.depth / MAX_STAR_DEPTH) * 255);
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);

            // Draw filled circle as star
            SDL_RenderFillCircle(renderer, star.x, star.y, starSize);
        }
#endif

        // fire effect
        fn_vDemoScene_Fire_Render(renderer);

        // plasma effect
        // we should do it on vessels !!!!!
        fn_vDemoScene_Plasma_RenderWithPalette(window, renderer, mPlayer.x, mPlayer.y, mPlayer.width, mPlayer.height);

        // Draw the player is not needed since it is the Plasma Effect now
//        SDL_Rect playerRect = { mPlayer.x, mPlayer.y, mPlayer.width, mPlayer.height };
//        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//        SDL_RenderFillRect(renderer, &playerRect);

        // Draw the player bullets
        //#pragma omp parallel for
        for (int i = 0; i < nbPlayerBullets; i++)
        {
            if (mPlayerBullet[i].active == 1)
            {
                SDL_Rect ballRect = { mPlayerBullet[i].x - mPlayerBullet[i].width,
                               mPlayerBullet[i].y - mPlayerBullet[i].height,
                               mPlayerBullet[i].width * 2, mPlayerBullet[i].height * 2 };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &ballRect);
            }
        }
        //iNb_Threads = omp_get_max_threads();

        // Draw the invaders
        //#pragma omp parallel for
        for (int i = 0; i < vInvaders.size(); i++)
        {
            if (!vInvaders[i].destroyed)
            {
                SDL_Rect blockRect = { vInvaders[i].x, vInvaders[i].y, vInvaders[i].width, vInvaders[i].height };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &blockRect);
            }
        }
        //iNb_Threads = omp_get_max_threads();

        // Draw the explosions
        for (int i = 0; i < INVADER_DESTRUCTION_MAX_COUNT; i++)
        {
            if (stExplosion_location_Elements[i].bActive)           // is it active?
            {
                if (stExplosion_location_Elements[i].bIgnite)       // did it start yet?
                {
                    fn_vRandomly_Init_Particles();                  // set effect
                    stExplosion_location_Elements[i].timer = 0;     // duration of destruction effect, so far
                    stExplosion_location_Elements[i].bIgnite = 0;   // tell it that it already started, no need for ignition process anymore
                }   
                fm_iRun_Explosion_RendererStyle_XY(renderer,
                    stExplosion_location_Elements[i].x, 
                    stExplosion_location_Elements[i].y, 
                    stExplosion_location_Elements[i].width, 
                    stExplosion_location_Elements[i].height);       // render effect
                stExplosion_location_Elements[i].timer++;           // increment timer
                if (stExplosion_location_Elements[i].timer > INVADER_DESTRUCTION_TIMER) 
                    stExplosion_location_Elements[i].bActive = 0;   // deactivate if past timer time !
            }
        }

        // calculate FPS
        iEndTime = SDL_GetPerformanceCounter();
        float elapsedTime = (iEndTime - iStartTime) / static_cast<float>(SDL_GetPerformanceFrequency());
        fEstimated_Fps = 1.0f / elapsedTime;

        // Render FPS
        fn_vDisplayFPS(renderer, fEstimated_Fps);

        // Display the window
        SDL_RenderPresent(renderer);

        // Delay to achieve desired frame rate
        SDL_Delay(frameDelay);
    }

#endif

    // Clean up SDL and SDL_TTF
    TTF_CloseFont(pstFont);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    // now we're done, we can retourn 0
    return 0;

}




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


