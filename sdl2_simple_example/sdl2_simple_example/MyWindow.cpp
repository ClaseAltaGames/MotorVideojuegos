#include "GL/glew.h"
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
bool abrirPopupAcercaDe = false;


bool isPlaying = false;
bool isPaused = false;
bool nextFrame = false;

bool fullScreen = true;

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
        if (ImGui::BeginMenu("Rockstallica Engine")) {
            if (ImGui::MenuItem("Acerca de...")) {
                abrirPopupAcercaDe = true;
            }
            if (ImGui::BeginMenu("Ajustes")) {
                if (ImGui::Checkbox("Mostrar FPS", &mostrarFPS));
                if (ImGui::Checkbox("Deteccion de hardware", &hardwareDetection));
                if (ImGui::Checkbox("Consumo de memoria", &memoryConsume));
                if (ImGui::Checkbox("Informacion de pantalla", &displayInfo));
				if (ImGui::Checkbox("Pantalla completa",&fullScreen));
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Salir del motor...")) {
                SDL_Event quit_event;
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("GameObjects")) {
            if (ImGui::MenuItem("Cubo")) {
                displayFunc->setActiveObject("cube");
            }
            if (ImGui::MenuItem("Piramide")) {
				displayFunc->setActiveObject("pyramid");
            }
            if (ImGui::MenuItem("Esfera")) {
				displayFunc->setActiveObject("sphere");
            }
            if (ImGui::MenuItem("Cylinder")) {
				displayFunc->setActiveObject("cylinder");
            }

            ImGui::EndMenu();
        }
        if (mostrarFPS) {
            ImGui::Begin("Ventana de FPS");

            // Obtener el FPS actual
            float fps = ImGui::GetIO().Framerate;

            // Tamaño del historial de FPS
            static const int fpsHistorySize = 100;
            static float fpsHistory[fpsHistorySize] = { 0.0f };
            static int fpsIndex = 0;

            // Añadir el FPS actual al historial, reemplazando el valor más antiguo
            fpsHistory[fpsIndex] = fps;
            fpsIndex = (fpsIndex + 1) % fpsHistorySize;

            // Mostrar el valor actual de FPS
            ImGui::Text("FPS: %.1f", fps);

            // Mostrar el gráfico de FPS con cubitos
            ImGui::PlotHistogram("FPS History", fpsHistory, fpsHistorySize, fpsIndex, nullptr, 0.0f, 100.0f, ImVec2(0, 80));

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
			// En ARM (Apple Silicon) solamente muestra la marca del procesador pero no los @GHz
            ImGui::Text("CPU: %s", ObtenerInfoCPU().c_str());

            ImGui::End();
        }
        if (displayInfo) {
            ImGui::Begin("Version del Programa");  // Ventana de versión
            ImGui::Text("Version 0.5 Beta");
            ImGui::End();
        }
		if (abrirPopupAcercaDe) {
			ImGui::OpenPopup("Acerca de...");
			abrirPopupAcercaDe = false;
		}
        if (fullScreen) {
            // Configurar pantalla completa con escalado
            SDL_SetWindowFullscreen(static_cast<SDL_Window*>(_window), SDL_WINDOW_FULLSCREEN_DESKTOP);

            // Obtener el tamaño actual del monitor
            int displayW, displayH;
            SDL_GetWindowSize(static_cast<SDL_Window*>(_window), &displayW, &displayH);

            // Actualizar el viewport de OpenGL
            glViewport(0, 0, displayW, displayH);
        }
        else {
            // Salir de pantalla completa y restaurar el tamaño original
            SDL_SetWindowFullscreen(static_cast<SDL_Window*>(_window), 0);
            SDL_SetWindowSize(static_cast<SDL_Window*>(_window), _width, _height);

            // Actualizar el viewport de OpenGL
            glViewport(0, 0, _width, _height);
        }

        if (ImGui::BeginPopup("Acerca de...")) {
            ImGui::Text("Rockstallica Engine version 0.5");
            ImGui::Text(" ");
            ImGui::Text("Todos los derechos reservados a Clase Alta Games");
            ImGui::Text(" ");
            ImGui::Text("Copyright: Clase Alta Games");
            ImGui::Text(" ");
            if (ImGui::Button("Visitar GitHub")) {
                system("start https://github.com/ClaseAltaGames/MotorVideojuegos");
            }
            ImGui::Text(" ");
			if (ImGui::Button("Pau Mena Torres")) {
				system("start https://github.com/PauMenaTorres");
			}
            ImGui::Text(" ");
			if (ImGui::Button("Edgar Mesa Dominguez")) {
				system("start https://github.com/edgarmd1");
			}
            ImGui::Text(" ");
            if (ImGui::Button("Cerrar")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
       
        ImGui::EndMainMenuBar();

        if (ImGui::TreeNode("Hierachy of GameObjects")) {
            if (displayFunc->cubeActive == true) {
                if (ImGui::TreeNode("Cubo"))
                {
                    if (ImGui::TreeNode("Propiedades del Cubo")) {
                        if (ImGui::TreeNode("Transform")) {
                            ImGui::Text("Nombre del archivo: Cubo precargado");
                            ImGui::Text("Posicion: (-10.0, 0.0, 0.0)");
                            ImGui::Text("Rotacion: (%.1f, %.1f, %.1f)");
                            ImGui::Text("Escala: %.1f", 1.0f);
                            ImGui::TreePop();
                        }
                        if (ImGui::TreeNode("Mesh")) {
                            ImGui::Text("Vertices: 24", 0);
                            ImGui::Text("Triangulos: 12", 0);
                            ImGui::TreePop();
                        }
                        if (ImGui::TreeNode("Texturas")) {
                            ImGui::Text("Nombre de la Textura: None");
                            ImGui::TreePop();
                        }
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
            }
			if (displayFunc->pyramidActive == true) {
				if (ImGui::TreeNode("Piramide"))
				{
					if (ImGui::TreeNode("Propiedades de la Piramide")) {
						if (ImGui::TreeNode("Transform")) {
							ImGui::Text("Nombre del archivo: Piramide precargada");
							//tamaño
							ImGui::Text("Posicion: (-5.0, 0.0, 0.0)");
							ImGui::Text("Rotacion: (%.1f, %.1f, %.1f)");
							ImGui::Text("Escala: %.1f", 1.0f);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Mesh")) {
							ImGui::Text("Vertices: 5");
							ImGui::Text("Caras: 6");
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Texturas")) {
							//ImGui::InputText("Nombre del archivo", currentFBXFile, 256);
							//nombre
							ImGui::Text("Nombre de la Textura: None");
							ImGui::TreePop();
						}
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
			}

			if (displayFunc->sphereActive == true) {
				if (ImGui::TreeNode("Esfera"))
				{
					if (ImGui::TreeNode("Propiedades de la Esfera")) {
						if (ImGui::TreeNode("Transform")) {
							ImGui::Text("Nombre del archivo: Esfera precargada");
							//tamaño
							ImGui::Text("Posicion: (5.0, 0.0, 0.0)");
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
							//nombre
							ImGui::Text("Nombre de la Textura: None");
							ImGui::TreePop();
						}
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
			}

			if (displayFunc->cylinderActive == true) {
				if (ImGui::TreeNode("Cylinder"))
				{
					if (ImGui::TreeNode("Propiedades del Cylinder")) {
						if (ImGui::TreeNode("Transform")) {
							ImGui::Text("Nombre del archivo: Cylinder precargado");
							//tamaño
							ImGui::Text("Posicion: (10.0, 0.0, 0.0)");
							ImGui::Text("Rotacion: (%.1f, %.1f, %.1f)");
							ImGui::Text("Escala: %.1f", 1.0f);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Mesh")) {
							ImGui::Text("Vertices: 74", 0);
							ImGui::Text("Caras: 144", 0);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Texturas")) {
							//ImGui::InputText("Nombre del archivo", currentFBXFile, 256);
							//nombre
							ImGui::Text("Nombre de la Textura: None");
							ImGui::TreePop();
						}
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
			}

			if (displayFunc->currentFBXFile.c_str() != NULL) {
				if (ImGui::TreeNode("GameObject")) {
					if (ImGui::TreeNode("Propiedades del GameObject")) {
						if (ImGui::TreeNode("Transform")) {
							//ImGui::InputText("Nombre del archivo", displayFunc->currentFBXFile.c_str(), 256);
							//tamaño
							ImGui::Text("Posicion: (%.1f, %.1f, %.1f)");
							ImGui::Text("Rotacion: (%.1f, %.1f, %.1f)");
							ImGui::Text("Escala: %.1f", 1.0f);
							//ImGui::SliderInt("Escala", &scale, 1, 3);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Mesh")) {
							ImGui::Text("Nombre del FBX %s", displayFunc->currentFBXFile.c_str());
							//meshes
							ImGui::Text("Vertices: %d", importerFBX->totalVertices);
							ImGui::Text("Triangulos: %d", importerFBX->totalFaces);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Texturas")) {
							//ImGui::InputText("Nombre del archivo", currentFBXFile, 256);
							//nombre
							ImGui::Text("Nombre de la Textura: %s", displayFunc->currentTextureFile.c_str());
							ImGui::TreePop();
						}
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
        }
    }
    // Barra inferior
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()), ImGuiCond_Always); // Justo debajo de la barra principal
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 0), ImGuiCond_Always); // Ancho completo

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    ImGui::Begin("Barra de control", nullptr, flags);

    // Centramos los botones
    float windowWidth = ImGui::GetWindowSize().x; // Ancho de la barra inferior
    float buttonWidth = 60.0f;                   // Ancho de cada botón (aproximado)
    float spacing = ImGui::GetStyle().ItemSpacing.x; // Espaciado entre botones
    float totalWidth = (buttonWidth * 3) + (spacing * 2); // Total: 3 botones + 2 espacios

    float offsetX = (windowWidth - totalWidth) / 2.0f; // Offset para centrar

    ImGui::SetCursorPosX(offsetX); // Mover el cursor al centro
    if (ImGui::Button("Play", ImVec2(buttonWidth, 0))) {
        displayFunc->paused = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause", ImVec2(buttonWidth, 0))) {
        displayFunc->paused = true; // Pausa o reanuda la simulación
    }
    ImGui::SameLine();
    if (ImGui::Button("Next", ImVec2(buttonWidth, 0))) {
        if (isPaused) {
            nextFrame = true; // Marca que debe avanzar un frame
        }
    }

    ImGui::End();


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




