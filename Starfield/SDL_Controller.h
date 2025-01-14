#pragma once


// 1. Initialize SDL
bool initializeSDL_Video_And_ControllerTypes();
// 2. Detect and Open Controllers
SDL_GameController* openController(int index);
// 3. Process Controller Events
void processControllerInput(SDL_GameController* controller);
// 4. Handle PS5 - Specific Features
void initializeHaptics(SDL_GameController* controller);
// 5. Cleanup Resources
void cleanup(SDL_GameController* controller);