#pragma once

#include "sdl.h"

SDL_GameController* open_controller(int joystick_index);
void close_controller(SDL_GameController* controller);
