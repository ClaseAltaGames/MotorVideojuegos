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

using namespace std;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(512, 512);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static void init_openGL() {
	glewInit();
	if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available.");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 1.0);
}

static void draw_triangle(const u8vec4& color, const vec3& center, double size) {
	glColor4ub(color.r, color.g, color.b, color.a);
	glBegin(GL_TRIANGLES);
	glVertex3d(center.x, center.y + size, center.z);
	glVertex3d(center.x - size, center.y - size, center.z);
	glVertex3d(center.x + size, center.y - size, center.z);
	glEnd();
}


static void draw_lines(const u8vec4& color, const vec3& center, double size) {
	glColor4ub(color.r, color.g, color.b, color.a);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 10.f, 0.f);
	glEnd();
}

static void draw_cube(const vec3& center, double size) {
    // Vértices del cubo
       // Vértices del cubo
    static const GLfloat v0[3] = { -1.0f, -1.0f,  1.0f };
    static const GLfloat v1[3] = { 1.0f, -1.0f,  1.0f };
    static const GLfloat v2[3] = { 1.0f,  1.0f,  1.0f };
    static const GLfloat v3[3] = { -1.0f,  1.0f,  1.0f };
    static const GLfloat v4[3] = { -1.0f, -1.0f, -1.0f };
    static const GLfloat v5[3] = { 1.0f, -1.0f, -1.0f };
    static const GLfloat v6[3] = { 1.0f,  1.0f, -1.0f };
    static const GLfloat v7[3] = { -1.0f,  1.0f, -1.0f };


    // Configurar la matriz de proyección
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.1, 100.0);  // Perspectiva con campo de visión de 45 grados

    // Configuración de la cámara
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  // La cámara se posiciona en (3, 3, 3)

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

static void display_func() {
    // Limpiar el buffer de color y profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Dibujar el cubo
    draw_cube(vec3(0.0, 0.0, 0.0), 1.0);

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