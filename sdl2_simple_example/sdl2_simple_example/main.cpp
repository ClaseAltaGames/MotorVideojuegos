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
using namespace std;

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64
#define FBX_FILE "Assets/halo2.fbx"

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(512, 512);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

ImporterFBX* importer = new ImporterFBX();

static void init_openGL() {
	glewInit();
	if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available.");
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
	glClearColor(0.5, 0.5, 0.5, 1.0);
}

// Variables globales para la posición y orientación de la cámara
GLdouble cameraPosX = 100.0, cameraPosY = 50.0, cameraPosZ = 100.0;
GLdouble cameraDirX = 0.0, cameraDirY = 0.0, cameraDirZ = 0.0;
GLdouble cameraUpX = 0.0, cameraUpY = 1.0, cameraUpZ = 0.0;

// Variables para near y far
GLdouble nearPlane = 0.1;
GLdouble farPlane = 200.0;

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
    nearPlane = max(0.1, cameraPosY / 10.0); // Asegurarse de que el plano cercano no sea demasiado pequeño
    farPlane = max(200.0, cameraPosZ); // Ajustar el plano lejano
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

GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
GLuint textureID;
static void generate_textures()
{
    for (int i = 0; i < CHECKERS_HEIGHT; i++) {
        for (int j = 0; j < CHECKERS_WIDTH; j++) {
            int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
            checkerImage[i][j][0] = (GLubyte)c;
            checkerImage[i][j][1] = (GLubyte)c;
            checkerImage[i][j][2] = (GLubyte)c;
            checkerImage[i][j][3] = (GLubyte)255;
        }
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
}
static void display_func() {
    // Limpiar el buffer de color y profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Dibujar el modelo FBX
    if (importer) {
        importer->draw_fbx(FBX_FILE);  // Llama a la función de la instancia de ImporterFBX para renderizar
    }

    set3dView();
    movimientoCamara();
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
			break;
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

    //draw_fbx("cube.fbx");

	while (processEvents()) {
		const auto t0 = hrclock::now();
		display_func();
		window.swapBuffers();
		const auto t1 = hrclock::now();
		const auto dt = t1 - t0;
		if(dt<FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
	}


    return 0;
}
