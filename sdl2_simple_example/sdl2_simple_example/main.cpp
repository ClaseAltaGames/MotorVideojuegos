#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include <SDL2/SDL_events.h>
#include "MyWindow.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdio.h>
#include "ImporterFBX.h"
#include "BasicForms.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <codecvt>
#include <locale>

using namespace std;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64
#define FBX_FILE "Assets/BakerHouse.fbx"
#define TEXTURE_FILE "Assets/Baker_house.png"

std::wstring charToWstring(const char* text) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(text);
}

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(512, 512);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

ImporterFBX* importer = new ImporterFBX();
BasicForms* basicForms = new BasicForms();

// Variables globales para la posición y orientación de la cámara
GLdouble cameraPosX = 5.0, cameraPosY = 5.0, cameraPosZ = 5.0;
GLdouble cameraDirX = 0.0, cameraDirY = 0.0, cameraDirZ = 0.0;
GLdouble cameraUpX = 0.0, cameraUpY = 1.0, cameraUpZ = 0.0;

// Variables para near y far
GLdouble nearPlane = 0.01;
GLdouble farPlane = 2000.0;


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

void set3dView() {
    // Fija el campo de visión a 45 grados
    const GLdouble fov = 45.0;

    // Configurar la matriz de proyección
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, 1.0, nearPlane, farPlane);

    // Configuración de la cámara
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ,
        cameraDirX, cameraDirY, cameraDirZ,
        cameraUpX, cameraUpY, cameraUpZ);
}

static void movimientoCamara() {
	set3dView();
    // Control de velocidad de movimiento y rotación
    const double moveSpeed = 2.0;
    const double rotationSpeed = 0.05;

    // Actualizar posición de la cámara con teclas de dirección
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_W]) {
        cameraPosX += moveSpeed * (cameraDirX - cameraPosX) * 0.1; // Ajustar el movimiento
        cameraPosZ += moveSpeed * (cameraDirZ - cameraPosZ) * 0.1; // Ajustar el movimiento
    }
    if (state[SDL_SCANCODE_S]) {
        cameraPosX -= moveSpeed * (cameraDirX - cameraPosX) * 0.1; // Ajustar el movimiento
        cameraPosZ -= moveSpeed * (cameraDirZ - cameraPosZ) * 0.1; // Ajustar el movimiento
    }
    if (state[SDL_SCANCODE_A]) {
        cameraPosX -= moveSpeed * (cameraDirZ - cameraPosZ) * 0.1; // Ajustar el movimiento
        cameraPosZ += moveSpeed * (cameraDirX - cameraPosX) * 0.1; // Ajustar el movimiento
    }
    if (state[SDL_SCANCODE_D]) {
        cameraPosX += moveSpeed * (cameraDirZ - cameraPosZ) * 0.1; // Ajustar el movimiento
        cameraPosZ -= moveSpeed * (cameraDirX - cameraPosX) * 0.1; // Ajustar el movimiento
    }

    // Rotar vista con teclas de flecha
    if (state[SDL_SCANCODE_LEFT]) {
        cameraDirX = cameraPosX + cos(rotationSpeed) * (cameraDirX - cameraPosX) - sin(rotationSpeed) * (cameraDirZ - cameraPosZ);
        cameraDirZ = cameraPosZ + sin(rotationSpeed) * (cameraDirX - cameraPosX) + cos(rotationSpeed) * (cameraDirZ - cameraPosZ);
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        cameraDirX = cameraPosX + cos(-rotationSpeed) * (cameraDirX - cameraPosX) - sin(-rotationSpeed) * (cameraDirZ - cameraPosZ);
        cameraDirZ = cameraPosZ + sin(-rotationSpeed) * (cameraDirX - cameraPosX) + cos(-rotationSpeed) * (cameraDirZ - cameraPosZ);
    }
    if (state[SDL_SCANCODE_UP]) {
        cameraPosY += moveSpeed;
    }
    if (state[SDL_SCANCODE_DOWN]) {
        cameraPosY -= moveSpeed;
    }

    // Ajustar near y far planes según la posición de la cámara
    nearPlane = 0.01; // Asegurarse de que el plano cercano no sea demasiado pequeño
    farPlane = 2000.0; // Ajustar el plano lejano
}

static void draw_cube(const vec3& center, double size) {
    // Vértices del cubo
    static const GLfloat v0[3] = { -1.0f, -1.0f,  1.0f };
    static const GLfloat v1[3] = { 1.0f, -1.0f,  1.0f };
    static const GLfloat v2[3] = { 1.0f,  1.0f,  1.0f };
    static const GLfloat v3[3] = { -1.0f,  1.0f,  1.0f };
    static const GLfloat v4[3] = { -1.0f, -1.0f, -1.0f };
    static const GLfloat v5[3] = { 1.0f, -1.0f, -1.0f };
    static const GLfloat v6[3] = { 1.0f,  1.0f, -1.0f };
    static const GLfloat v7[3] = { -1.0f,  1.0f, -1.0f };

    set3dView();

    glBegin(GL_TRIANGLES);  // Dibujar el cubo con triángulos

    // Cara frontal (v0, v1, v2, v3) - Color: Verde
    glColor4ub(0, 255, 0, 255); // Verde
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2);  // Primer triángulo
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v0);  // Segundo triángulo

    // Cara derecha (v1, v5, v6, v2) - Color: Azul
    glColor4ub(0, 0, 255, 255); // Azul
    glVertex3fv(v1); glVertex3fv(v5); glVertex3fv(v6);  // Primer triángulo
    glVertex3fv(v6); glVertex3fv(v2); glVertex3fv(v1);  // Segundo triángulo

    // Cara trasera (v5, v4, v7, v6) - Color: Rojo
    glColor4ub(255, 0, 0, 255); // Rojo
    glVertex3fv(v5); glVertex3fv(v4); glVertex3fv(v7);  // Primer triángulo
    glVertex3fv(v7); glVertex3fv(v6); glVertex3fv(v5);  // Segundo triángulo

    // Cara izquierda (v4, v0, v3, v7) - Color: Amarillo
    glColor4ub(255, 255, 0, 255); // Amarillo
    glVertex3fv(v4); glVertex3fv(v0); glVertex3fv(v3);  // Primer triángulo
    glVertex3fv(v3); glVertex3fv(v7); glVertex3fv(v4);  // Segundo triángulo

    // Cara superior (v3, v2, v6, v7) - Color: Naranja
    glColor4ub(255, 165, 0, 255); // Naranja
    glVertex3fv(v3); glVertex3fv(v2); glVertex3fv(v6);  // Primer triángulo
    glVertex3fv(v6); glVertex3fv(v7); glVertex3fv(v3);  // Segundo triángulo

    // Cara inferior (v4, v5, v1, v0) - Color: Cian
    glColor4ub(0, 255, 255, 255); // Cian
    glVertex3fv(v4); glVertex3fv(v5); glVertex3fv(v1);  // Primer triángulo
    glVertex3fv(v1); glVertex3fv(v0); glVertex3fv(v4);  // Segundo triángulo

    glEnd();
}
static void draw_pyramid(const vec3& center, double size) {
    // Vértices de la pirámide
    static const GLfloat v0[3] = { -1.0f, -1.0f,  1.0f }; // base inferior izquierda
    static const GLfloat v1[3] = { 1.0f, -1.0f,  1.0f }; // base inferior derecha
    static const GLfloat v2[3] = { 1.0f, -1.0f, -1.0f }; // base superior derecha
    static const GLfloat v3[3] = { -1.0f, -1.0f, -1.0f }; // base superior izquierda
    static const GLfloat top[3] = { 0.0f,  1.0f,  0.0f }; // vértice superior
    set3dView();
    glBegin(GL_TRIANGLES);  // Dibujar la pirámide con triángulos
    // Cara frontal (v0, v1, top) - Color: Rojo
    glColor4ub(255, 0, 0, 255);
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(top);
    // Cara derecha (v1, v2, top) - Color: Verde
    glColor4ub(0, 255, 0, 255);
    glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(top);
    // Cara trasera (v2, v3, top) - Color: Azul
    glColor4ub(0, 0, 255, 255);
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(top);
    // Cara izquierda (v3, v0, top) - Color: Amarillo
    glColor4ub(255, 255, 0, 255);
    glVertex3fv(v3); glVertex3fv(v0); glVertex3fv(top);
    // Base cuadrada (v0, v1, v2, v3) - Color: Cian
    glColor4ub(0, 255, 255, 255);
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2); // Primer triángulo
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v0); // Segundo triángulo
    glEnd();
}



GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
GLuint textureID;

static void generate_textures(const char* filePath) {
    ILuint imageID;

    // Inicializa DevIL (esto debe hacerse solo una vez en el programa)
    ilInit();

    // Genera y enlaza un identificador de imagen de DevIL
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    // Convertir const char* a const wchar_t*
    //std::wstring wideFilePath = charToWstring(filePath);

    // Carga la imagen con DevIL

    if (ilLoadImage((const wchar_t *) filePath)) {
        // Configura parámetros de la textura en OpenGL
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Transferencia de datos de imagen de DevIL a OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
            0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
        glGenerateMipmap(GL_TEXTURE_2D);

        // Borra la imagen de DevIL de la memoria
        ilDeleteImages(1, &imageID);
    }
    else {
        // Imprimir el error de DevIL
        ILenum error = ilGetError();
        std::cerr << "DevIL error: " << iluErrorString(error) << "\n";
        std::cerr << "Failed to load texture: " << filePath << "\n";
        std::cerr << "DevIL error: " << iluErrorString(error) << "\n";
    }
}

static void display_func() {
    // Limpiar el buffer de color y profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Dibujar el modelo FBX
    if (importer) {
        importer->draw_fbx(FBX_FILE);  // Llama a la función de la instancia de ImporterFBX para renderizar
    }
	basicForms->cambiarFormas(CUBO);
   
    movimientoCamara();
	//generate_textures(TEXTURE_FILE);
    // Forzar el renderizado
    glFlush();
}

static bool processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) return false;
            if (event.key.keysym.sym == SDLK_f) {
                // Reiniciar parámetros a los iniciales
                cameraPosX = 5.0;
                cameraPosY = 5.0;
                cameraPosZ = 5.0;
                cameraDirX = 0.0;
                cameraDirY = 0.0;
                cameraDirZ = 0.0;
                cameraUpX = 0.0;
                cameraUpY = 1.0;
                cameraUpZ = 0.0;
                nearPlane = 0.01;
                farPlane = 2000.0;
            }
            break;
		case SDL_DROPFILE: {
			// Cargar el archivo FBX
			importer->draw_fbx(event.drop.file);
            printf("%s\n", event.drop.file);
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

    init_openGL();
	init_devil();

    while (processEvents()) {
		SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
        const auto t0 = hrclock::now();
        display_func();
        window.swapBuffers();
        const auto t1 = hrclock::now();
        const auto dt = t1 - t0;
        if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
    }

    return 0;
}
