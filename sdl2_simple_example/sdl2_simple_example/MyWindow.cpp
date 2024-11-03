#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include "MyWindow.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL_events.h>
using namespace std;


bool mostrarFPS = false;


MyWindow::MyWindow(const std::string& title, int w, int h) : _width(w), _height(h) {

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL);
    if (!_window) throw exception(SDL_GetError());

    _ctx = SDL_GL_CreateContext(_window);
    if (!_ctx) throw exception(SDL_GetError());
    if (SDL_GL_MakeCurrent(_window, _ctx) != 0) throw exception(SDL_GetError());
    if (SDL_GL_SetSwapInterval(1) != 0) throw exception(SDL_GetError());

    
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(_window, _ctx);
    ImGui_ImplOpenGL3_Init("#version 130");

	
}


MyWindow::~MyWindow() {
    ImGui_ImplSDL2_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(_ctx);
    SDL_DestroyWindow(static_cast<SDL_Window*>(_window));
}

void MyWindow::swapBuffers() const {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
            if (ImGui::MenuItem("Adios")) {
                SDL_Event quit_event;
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Objetos 3D")) {
            if (ImGui::MenuItem("Cubo")) {
				displayFunc->cubeActive = true;
            }
			if (ImGui::MenuItem("Piramide")) {
				displayFunc->pyramidActive = true;
			}
			if (ImGui::MenuItem("Esfera")) {
				displayFunc->sphereActive = true;
			}
            if (ImGui::MenuItem("Cylinder")) {
                displayFunc->cylinderActive = true;
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Acerca de")) {
            if (ImGui::MenuItem("GitHub")) {
				//formato para poder abrir paginas web en el navegador del ordenador (no se si se puede hacer de otra forma)
                system("start https://github.com/ClaseAltaGames/MotorVideojuegos");
            }
			if (ImGui::BeginMenu("Miembros")) {
                if (ImGui::MenuItem("Pau Mena")) {
                    system("start https://github.com/PauMenaTorres");
                }
                if (ImGui::MenuItem("Edgar Mesa")) {
                    system("start https://github.com/edgarmd1");
                }
				ImGui::EndMenu();
			}
            ImGui::EndMenu();
        }
		if (ImGui::BeginMenu("Ajustes")) {
            if (ImGui::Checkbox("Mostrar FPS", &mostrarFPS));
			ImGui::EndMenu();
		}
        if (mostrarFPS) {
            ImGui::Begin("Ventana de FPS");

            // Obtener el FPS actual y mostrarlo
            float fps = ImGui::GetIO().Framerate;
            ImGui::Text("FPS: %.1f", fps);

            ImGui::End();
        }
        ImGui::EndMainMenuBar();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(static_cast<SDL_Window*>(_window));
}


