#include "SDL_Controller.h"
#include <iostream>

SDL_GameController* open_controller(int joystick_index) {
    if (SDL_IsGameController(joystick_index)) {
        SDL_GameController* controller = SDL_GameControllerOpen(joystick_index);
        if (controller) {
            std::cout << "Found a compatible controller: " << SDL_GameControllerName(controller) << std::endl;
            return controller;
        }
        else {
            std::cerr << "Could not open gamecontroller " << joystick_index << ": " << SDL_GetError() << std::endl;
        }
    }
    return nullptr;
}

void close_controller(SDL_GameController* controller) {
    if (controller) {
        SDL_GameControllerClose(controller);
    }
}