#define SDL_MAIN_HANDLED  

#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include <SDL2/SDL_events.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>

//Clases
#include "MyWindow.h"
#include "ImporterFBX.h"
#include "BasicForms.h"
#include "DisplayFunc.h"
#include "Camera.h"
#include "Input.h"
#include "Textures.h"

using namespace std;

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(1024, 768);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

DisplayFunc* displayFunc = new DisplayFunc();
ImporterFBX* importerFBX = new ImporterFBX();
Camera* camera = new Camera();
Input* input = new Input();
Textures* textures = new Textures();

void init_openGL() {
    glewInit();
    if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available.");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.5, 0.5, 0.5, 1.0);
    printf("OpenGL version: %s, GLSL version: %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("OpenGL cargada exitosamente!\n");
}

void init_devil() {
    ilInit();
    iluInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);
    printf("Devil version: 1.8.0\n");
    printf("Devil cargada exitosamente!\n");
}

void getSDLVersion() {
    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    printf("Compiled against SDL version %d.%d.%d\n", compiled.major, compiled.minor, compiled.patch);
    printf("Linked against SDL version %d.%d.%d\n", linked.major, linked.minor, linked.patch);
}

GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

static bool processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            return false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) return false;
            break;
        case SDL_DROPFILE: {
            string droppedFile = event.drop.file;
            //esto es para que sepa que tipo de archivo es el que le estas arrastrando (comprobando que hay despues del .)
            string extension = droppedFile.substr(droppedFile.find_last_of('.') + 1);
            if (extension == "fbx" || extension == "FBX") {
                // Cargar el archivo FBX
                displayFunc->currentFBXFile = droppedFile;
                importerFBX->draw_fbx(displayFunc->currentFBXFile.c_str());
                printf("Objeto FBX cargado: %s\n", displayFunc->currentFBXFile.c_str());
            }
            else if (extension == "png" || extension == "PNG" || extension == "DDS" || extension == "dds") {
                // Cargar la textura PNG o DDS
                displayFunc->currentTextureFile = droppedFile;
                textures->LoadTexture(displayFunc->currentTextureFile.c_str());
            }

            printf("%s\n", event.drop.file);
            SDL_free(event.drop.file);
            break;
        }
        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0) {
                std::cout << "Rueda del ratón hacia arriba" << std::endl;
                camera->ZoomOUT();
            }
            else if (event.wheel.y < 0) {
                std::cout << "Rueda del ratón hacia abajo" << std::endl;
                camera->ZoomIN();
            }
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                input->SetMouseButton(LEFT_BUTTON, KEY_DOWN);  // Actualiza el estado al presionar
            }
            if (event.button.button == SDL_BUTTON_RIGHT) {
                input->SetMouseButton(RIGHT_BUTTON, KEY_DOWN);
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                input->SetMouseButton(LEFT_BUTTON, KEY_UP);  // Actualiza el estado al soltar
            }
            if (event.button.button == SDL_BUTTON_RIGHT) {
                input->SetMouseButton(RIGHT_BUTTON, KEY_UP);
            }
            break;
        }
    }
    return true;
}

int main(int argc, char** argv) {
    MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
    window.displayFunc = displayFunc;
    window.importerFBX = importerFBX;

    //iniciamos las librerias
    init_openGL();
    init_devil();
    getSDLVersion();

    printf("Objeto FBX cargado: %s\n", displayFunc->currentFBXFile.c_str());

    //bucle principal
    while (processEvents()) {
        SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
        const auto t0 = hrclock::now();
        displayFunc->DisplayALL();
        window.swapBuffers();
        const auto t1 = hrclock::now();
        const auto dt = t1 - t0;
        if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
    }

    return 0;
}
