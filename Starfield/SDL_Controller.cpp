#include <SDL.h>
#include <iostream>


#include "SDL_Controller.h"



// 1. Initialize SDL
// Ensure that SDL's subsystems for joystick and game controller handling are initialized.


bool initializeSDL_Video_And_ControllerTypes() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) != 0) {
        std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}



// 2. Detect and Open Controllers
// Scan for connected controllers and open them for input handling.

SDL_GameController* openController(int index) {
    if (SDL_IsGameController(index)) {
        SDL_GameController* controller = SDL_GameControllerOpen(index);
        if (controller) {
            std::cout << "Opened controller: " << SDL_GameControllerName(controller) << std::endl;
            return controller;
        }
        else {
            std::cerr << "Could not open game controller: " << SDL_GetError() << std::endl;
        }
    }
    return nullptr;
}


// 3. Process Controller Events
// Handle input events from the controllers, including button presses, axis movements, and other inputs.


void processControllerInput(SDL_GameController* controller) {
    SDL_Event event;
#if 0
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_CONTROLLERBUTTONDOWN) {
            std::cout << "Button "
                << SDL_GameControllerGetStringForButton(static_cast<SDL_GameControllerButton>(event.cbutton.button))
                << " pressed" << std::endl;
        }
        if (event.type == SDL_CONTROLLERBUTTONUP) {
            std::cout << "Button "
                << SDL_GameControllerGetStringForButton(static_cast<SDL_GameControllerButton>(event.cbutton.button))
                << " released" << std::endl;
        }
        if (event.type == SDL_CONTROLLERAXISMOTION) {
            std::cout << "Axis "
                << SDL_GameControllerGetStringForAxis(static_cast<SDL_GameControllerAxis>(event.caxis.axis))
                << " moved to " << event.caxis.value << std::endl;
        }
    }
#endif
    while (SDL_PollEvent(&event)) 
    {
        // Handle button presses
        if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP) {
            const char* action = (event.type == SDL_CONTROLLERBUTTONDOWN) ? "pressed" : "released";

            // Check specific buttons
            SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(event.cbutton.button);
            if (button == SDL_CONTROLLER_BUTTON_Y) { // Triangle button
                std::cout << "Triangle button " << action << std::endl;
            }
            else if (button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
                std::cout << "D-Pad Left " << action << std::endl;
            }
            else if (button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
                std::cout << "D-Pad Right " << action << std::endl;
            }
        }

        // Handle axis (stick) movements
        if (event.type == SDL_CONTROLLERAXISMOTION) {
            SDL_GameControllerAxis axis = static_cast<SDL_GameControllerAxis>(event.caxis.axis);
            int value = event.caxis.value; // Axis value (-32768 to 32767)

            // Detect significant movement (ignore small drifts with a deadzone threshold)
            const int deadzone = 8000; // Adjust as needed
            if (axis == SDL_CONTROLLER_AXIS_LEFTX) {
                if (value < -deadzone) {
                    std::cout << "Left Stick moved left with value: " << value << std::endl;
                }
                else if (value > deadzone) {
                    std::cout << "Left Stick moved right with value: " << value << std::endl;
                }
            }
            else if (axis == SDL_CONTROLLER_AXIS_RIGHTX) {
                if (value < -deadzone) {
                    std::cout << "Right Stick moved left with value: " << value << std::endl;
                }
                else if (value > deadzone) {
                    std::cout << "Right Stick moved right with value: " << value << std::endl;
                }
            }
        }
    }
}


// 4. Handle PS5 - Specific Features
// SDL recognizes the PS5 controller as a standard game controller, but to access special features(e.g., haptic feedback, touchpad) :

//    Haptic Feedback : Use SDL_Haptic to control the controller's vibration motors.
//    Touchpad Input : Currently, SDL doesnft natively support the PS5 touchpad.For advanced features, you may need platform - specific extensions or raw HID handling.


void initializeHaptics(SDL_GameController* controller) {
    SDL_Haptic* haptic = SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(controller));
    if (haptic) {
        if (SDL_HapticRumbleInit(haptic) == 0) {
            SDL_HapticRumblePlay(haptic, 0.75, 2000); // 75% intensity for 2 seconds
        }
        SDL_HapticClose(haptic);
    }
    else {
        std::cerr << "Haptic feedback not supported: " << SDL_GetError() << std::endl;
    }
}




// 5. Cleanup Resources
// Release resources when done.

void cleanup(SDL_GameController* controller) {
    if (controller) {
        SDL_GameControllerClose(controller);
    }
    SDL_Quit();
}



// code for main application
// 6. Main Application
// Combine all these steps in the main loop :

/*
int main(int argc, char* argv[]) {
    if (!initializeSDL()) {
        return -1;
    }

    SDL_GameController* controller = nullptr;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        controller = openController(i);
        if (controller) {
            break; // Assume we only care about the first controller for this example
        }
    }

    if (!controller) {
        std::cerr << "No compatible controller found!" << std::endl;
        SDL_Quit();
        return -1;
    }

    bool running = true;
    while (running) {
        processControllerInput(controller);

        // Exit condition (e.g., pressing ESC or a specific button)
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_ESCAPE]) {
            running = false;
        }
    }

    cleanup(controller);
    return 0;
}
*/









    






