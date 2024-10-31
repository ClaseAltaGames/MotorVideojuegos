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

//Clases
#include "MyWindow.h"
#include "ImporterFBX.h"
#include "BasicForms.h"
#include "DisplayFunc.h"

using namespace std;

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(512, 512);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

DisplayFunc* displayFunc = new DisplayFunc();
ImporterFBX* importerFBX = new ImporterFBX();

void init_openGL() {
    glewInit();
    if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available.");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.5, 0.5, 0.5, 1.0);

}
void init_devil() {
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

static bool processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) return false;
            break;
        case SDL_DROPFILE: {
            // Cargar el archivo FBX arrastrado
            displayFunc->currentFBXFile = event.drop.file;
            importerFBX->draw_fbx(displayFunc->currentFBXFile.c_str());
            SDL_free(event.drop.file);
            break;
        }
        default: {
            ImGui_ImplSDL2_ProcessEvent(&event);
            break;
        }
        }
    }
    return true;
}

int main(int argc, char** argv) {
    MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
	window.displayFunc = displayFunc;

    init_openGL();
	init_devil();

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
