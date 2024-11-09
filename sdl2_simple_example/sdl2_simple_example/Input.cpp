#include "Input.h"
#include "SDL2/SDL.h"
#include <cstring>

#define MAX_KEYS 300
#define NUM_MOUSE_BUTTONS 5

Input::Input(bool startEnabled) {
    keyboard = new KeyState[MAX_KEYS];
    memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
    memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}


Input::~Input()
{
    delete[] keyboard;
}

// Called before render is available
bool Input::Awake()
{
    bool ret = true;
    if (SDL_Init(0) < 0) {
        ret = false;
    }

    if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0) {
        ret = false;
    }

    return ret;
}

// Called before the first frame
bool Input::Start()
{
    SDL_StopTextInput();
    return true;
}

// Called each loop iteration
bool Input::PreUpdate()
{
    static SDL_Event event;

    // Obtener estado del teclado
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    for (int i = 0; i < MAX_KEYS; ++i)
    {
        if (keys[i] == 1)
        {
            if (keyboard[i] == KEY_IDLE)
                keyboard[i] = KEY_DOWN;
            else
                keyboard[i] = KEY_REPEAT;
        }
        else
        {
            if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
                keyboard[i] = KEY_UP;
            else
                keyboard[i] = KEY_IDLE;
        }
    }

    // Actualizar estado de los botones del ratón
    for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
    {
        if (mouseButtons[i] == KEY_DOWN)
            mouseButtons[i] = KEY_REPEAT;

        if (mouseButtons[i] == KEY_UP)
            mouseButtons[i] = KEY_IDLE;
    }

    // Procesar eventos SDL
    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            windowEvents[WE_QUIT] = true;
            break;

        case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_HIDDEN:
            case SDL_WINDOWEVENT_MINIMIZED:
            case SDL_WINDOWEVENT_FOCUS_LOST:
                windowEvents[WE_HIDE] = true;
                break;

            case SDL_WINDOWEVENT_SHOWN:
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            case SDL_WINDOWEVENT_MAXIMIZED:
            case SDL_WINDOWEVENT_RESTORED:
                windowEvents[WE_SHOW] = true;
                break;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button <= NUM_MOUSE_BUTTONS)
                mouseButtons[event.button.button - 1] = KEY_DOWN;
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button <= NUM_MOUSE_BUTTONS)
                mouseButtons[event.button.button - 1] = KEY_UP;
            break;

        case SDL_MOUSEMOTION:
            mouseMotionX = event.motion.xrel;
            mouseMotionY = event.motion.yrel;
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            break;
        }
    }

    return true;
}

// Called before quitting
bool Input::CleanUp()
{
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    return true;
}

bool Input::GetWindowEvent(EventWindow ev)
{
    return windowEvents[ev];
}

void Input::GetMousePosition(int& x, int& y)
{
    x = mouseX;
    y = mouseY;
}

void Input::GetMouseMotion(int& x, int& y)
{
    x = mouseMotionX;
    y = mouseMotionY;
}

KeyState Input::GetMouseButton(MouseButton button)
{
    if (button < NUM_MOUSE_BUTTONS)
        return mouseButtons[button];
    else
        return KEY_IDLE;
}
void Input::SetMouseButton(MouseButton button, KeyState state) {
    if (button >= 0 && button < NUM_MOUSE_BUTTONS) {
        mouseButtons[button] = state;
    }
}
