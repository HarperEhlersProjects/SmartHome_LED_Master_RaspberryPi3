#include "gamepad.h"

void GamePads::openControllers(void)
{
    int j = 0;

    numberConectedJoysticks = 0;
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i) && j<4) 
        {
            controllers[j] = SDL_GameControllerOpen(i);
            j++;
        }

        numberConectedJoysticks++;
    }

}

void GamePads::resetControllers()
{
    for (int i = 0; i < 4; i++)
    {
        controllers[i] = NULL;
        inputs[i] = { false,false,false,false,false,false,false,false,false,false,false,false };
    }
}

void GamePads::updateControllers(void)
{
    if(numberConectedJoysticks != SDL_NumJoysticks())
    {
        resetControllers();
        openControllers();
    }

    SDL_GameControllerUpdate();

    for(int i=0;i<4;i++)
    { 
        if (controllers[i] != NULL)
        {
            inputs[i].SELECT = SDL_GameControllerGetButton(controllers[i], SDL_CONTROLLER_BUTTON_BACK);
            inputs[i].START = SDL_GameControllerGetButton(controllers[i], SDL_CONTROLLER_BUTTON_START);

            inputs[i].B = SDL_GameControllerGetButton(controllers[i], SDL_CONTROLLER_BUTTON_A);
            inputs[i].A = SDL_GameControllerGetButton(controllers[i], SDL_CONTROLLER_BUTTON_B);

            inputs[i].X = SDL_GameControllerGetButton(controllers[i], SDL_CONTROLLER_BUTTON_Y);
            inputs[i].Y = SDL_GameControllerGetButton(controllers[i], SDL_CONTROLLER_BUTTON_X);

            inputs[i].UP = SDL_GameControllerGetButton(controllers[i], SDL_CONTROLLER_BUTTON_DPAD_UP);
            inputs[i].DOWN = SDL_GameControllerGetButton(controllers[i], SDL_CONTROLLER_BUTTON_DPAD_DOWN);

            inputs[i].LEFT = SDL_GameControllerGetButton(controllers[i], SDL_CONTROLLER_BUTTON_DPAD_LEFT);
            inputs[i].RIGHT = SDL_GameControllerGetButton(controllers[i], SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

            inputs[i].L = SDL_GameControllerGetButton(controllers[i], SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
            inputs[i].R = SDL_GameControllerGetButton(controllers[i], SDL_CONTROLLER_BUTTON_MAX);
        }
    } 
}