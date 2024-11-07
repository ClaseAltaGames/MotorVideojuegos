#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include "MyWindow.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL_events.h>
#include <windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
using namespace std;


bool mostrarFPS = false;
bool hardwareDetection = false;
bool memoryConsume = false;
bool displayInfo = false;


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
			if (ImGui::Checkbox("Deteccion de hardware", &hardwareDetection));
			if (ImGui::Checkbox("Consumo de memoria", &memoryConsume));
			if (ImGui::Checkbox("Informacion de pantalla", &displayInfo));
			ImGui::EndMenu();
		}
        if (mostrarFPS) {
            ImGui::Begin("Ventana de FPS");

            // Obtener el FPS actual y mostrarlo
            float fps = ImGui::GetIO().Framerate;
            ImGui::Text("FPS: %.1f", fps);

            ImGui::End();
        }
        if (memoryConsume) {
            ImGui::Begin("Ventana de Consumo de Memoria");

            MEMORYSTATUSEX memInfo;
            memInfo.dwLength = sizeof(MEMORYSTATUSEX);
            GlobalMemoryStatusEx(&memInfo);

            DWORDLONG totalMemoria = memInfo.ullTotalPhys / (1024 * 1024);  // Total de RAM en MB
            DWORDLONG memoriaEnUso = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024 * 1024);  // RAM en uso en MB

            ImGui::Text("Memoria Total: %llu MB", totalMemoria);
            ImGui::Text("Memoria en Uso: %llu MB", memoriaEnUso);
            ImGui::End();
        }
        if (hardwareDetection) {
            ImGui::Begin("Info de Hardware");

            // Muestra el nombre de la CPU
            ImGui::Text("CPU: %s", ObtenerInfoCPU().c_str());

            ImGui::End();
        }
        if (displayInfo) {
            ImGui::Begin("Version del Programa");  // Ventana de versión
            ImGui::Text("Version 0.5 Beta");
            ImGui::End();
        }
       
        ImGui::EndMainMenuBar();

        if (ImGui::TreeNode("Hierachy of GameObjects")) {
            if (displayFunc->cubeActive == true) {
                if (ImGui::TreeNode("Cubo"))
                {
                    if (ImGui::TreeNode("Propiedades del Cubo")) {
                        if (ImGui::TreeNode("Transform")) {
                            ImGui::Text("Nombre del archivo: Cubo precargado");
                            //tamaño
                            ImGui::Text("Posicion: (%.1f, %.1f, %.1f)");
                            ImGui::Text("Rotacion: (%.1f, %.1f, %.1f)");
                            ImGui::Text("Escala: %.1f", 1.0f);
                            ImGui::TreePop();
                        }
                        if (ImGui::TreeNode("Mesh")) {
                            ImGui::Text("Vertices: %d", 0);
                            ImGui::Text("Caras: %d", 0);
                            ImGui::TreePop();
                        }
                        if (ImGui::TreeNode("Texturas")) {
                            //ImGui::InputText("Nombre del archivo", currentFBXFile, 256);
                            //tamaño
                            ImGui::Text("Nombre de la Textura: None");
                            ImGui::TreePop();
                        }
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
            }
			ImGui::TreePop();
        }
   //     if (ImGui::TreeNode("Propiedades del GameObject")) {
   //         
   //         if (ImGui::TreeNode("Transform")) {
   //             //ImGui::InputText("Nombre del archivo", currentFBXFile, 256);
   //             //tamaño
   //             ImGui::Text("Posicion: (%.1f, %.1f, %.1f)");
   //             ImGui::Text("Rotacion: (%.1f, %.1f, %.1f)");
   //             ImGui::Text("Escala: %.1f", 1.0f);


   //             //ImGui::SliderInt("Escala", &scale, 1, 3);
   //             ImGui::TreePop();
   //         }
			//if (ImGui::TreeNode("Mesh")) {
			//	ImGui::Text("Nombre del FBX %s", displayFunc->currentFBXFile.c_str());
			//	//tamaño
			//	ImGui::Text("Vertices: %d", 0);
			//	ImGui::Text("Caras: %d", 0);
			//	ImGui::Text("Texturas: %d", 0);
			//	ImGui::TreePop();
			//}
			//if (ImGui::TreeNode("Texturas")) {
			//	//ImGui::InputText("Nombre del archivo", currentFBXFile, 256);
			//	//tamaño
			//	ImGui::Text("Nombre de la Textura: %s", displayFunc->currentTextureFile.c_str());
			//	ImGui::TreePop();
			//}
			//ImGui::TreePop();
   //     }
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(static_cast<SDL_Window*>(_window));
}

std::string cachedCPUInfo;  // Variable para almacenar la información del CPU
bool isCPUInfoCached = false;

string MyWindow::ObtenerInfoCPU() const {
    if (isCPUInfoCached) {
        return cachedCPUInfo;  // Devuelve la información almacenada si ya ha sido consultada
    }

    IWbemLocator* locator = nullptr;
    IWbemServices* services = nullptr;
    CoInitializeEx(0, COINIT_MULTITHREADED);
    CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE, nullptr);

    HRESULT hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&locator);
    if (FAILED(hres)) return "Error al inicializar WMI.";

    hres = locator->ConnectServer(BSTR(L"ROOT\\CIMV2"), nullptr, nullptr, nullptr, 0, nullptr, nullptr, &services);
    if (FAILED(hres)) return "Error al conectar a WMI.";

    IEnumWbemClassObject* enumerator = nullptr;
    hres = services->ExecQuery(BSTR(L"WQL"), BSTR(L"SELECT Name FROM Win32_Processor"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &enumerator);
    if (FAILED(hres)) return "Error al ejecutar consulta WMI.";

    IWbemClassObject* obj = nullptr;
    ULONG returnedCount = 0;
    enumerator->Next(WBEM_INFINITE, 1, &obj, &returnedCount);

    VARIANT name;
    obj->Get(L"Name", 0, &name, nullptr, nullptr);

    cachedCPUInfo = string(_bstr_t(name.bstrVal).operator const char* ());  // Guardar el resultado
    isCPUInfoCached = true;  // Marcar como cacheado

    VariantClear(&name);
    obj->Release();
    enumerator->Release();
    services->Release();
    locator->Release();
    CoUninitialize();

    return cachedCPUInfo;
}

int MyWindow::GetScale() const
{
    return scale;
}




