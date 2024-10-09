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

void set3dView(GLdouble angle) {
    // Configurar la matriz de proyección
    GLdouble yView = 50.0;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, 1.0, 0.1, 200.0);  // Perspectiva con campo de visión de 45 grados

    // Configuración de la cámara
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //if mouse wheel up
    if (SDL_SCANCODE_UP) {
        yView += 10;
	}
    if (SDL_SCANCODE_DOWN) {
        yView -= 10;
    }

    gluLookAt(100.0, yView, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  // La cámara se posiciona en (3, 3, 3)

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
    static const GLfloat v0[3] = { -1.0f, -1.0f,  1.0f };
    static const GLfloat v1[3] = { 1.0f, -1.0f,  1.0f };
    static const GLfloat v2[3] = { 1.0f,  1.0f,  1.0f };
    static const GLfloat v3[3] = { -1.0f,  1.0f,  1.0f };
    static const GLfloat v4[3] = { -1.0f, -1.0f, -1.0f };
    static const GLfloat v5[3] = { 1.0f, -1.0f, -1.0f };
    static const GLfloat v6[3] = { 1.0f,  1.0f, -1.0f };
    static const GLfloat v7[3] = { -1.0f,  1.0f, -1.0f };

	set3dView(45);

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

	set3dView(45);

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


static void draw_prism(const vec3& center, double width, double height, double depth) {
    // Vértices del prisma rectangular
    static const GLfloat v0[3] = { -width, -height,  depth };
    static const GLfloat v1[3] = { width, -height,  depth };
    static const GLfloat v2[3] = { width,  height,  depth };
    static const GLfloat v3[3] = { -width,  height,  depth };
    static const GLfloat v4[3] = { -width, -height, -depth };
    static const GLfloat v5[3] = { width, -height, -depth };
    static const GLfloat v6[3] = { width,  height, -depth };
    static const GLfloat v7[3] = { -width,  height, -depth };

	set3dView(45);

    glBegin(GL_TRIANGLES);  // Dibujar el prisma con triángulos

    // Cara frontal (v0, v1, v2, v3) - Color: Rojo
    glColor4ub(255, 0, 0, 255);
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2);
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v0);

    // Cara derecha (v1, v5, v6, v2) - Color: Verde
    glColor4ub(0, 255, 0, 255);
    glVertex3fv(v1); glVertex3fv(v5); glVertex3fv(v6);
    glVertex3fv(v6); glVertex3fv(v2); glVertex3fv(v1);

    // Cara trasera (v5, v4, v7, v6) - Color: Azul
    glColor4ub(0, 0, 255, 255);
    glVertex3fv(v5); glVertex3fv(v4); glVertex3fv(v7);
    glVertex3fv(v7); glVertex3fv(v6); glVertex3fv(v5);

    // Cara izquierda (v4, v0, v3, v7) - Color: Amarillo
    glColor4ub(255, 255, 0, 255);
    glVertex3fv(v4); glVertex3fv(v0); glVertex3fv(v3);
    glVertex3fv(v3); glVertex3fv(v7); glVertex3fv(v4);

    // Cara superior (v3, v2, v6, v7) - Color: Naranja
    glColor4ub(255, 165, 0, 255);
    glVertex3fv(v3); glVertex3fv(v2); glVertex3fv(v6);
    glVertex3fv(v6); glVertex3fv(v7); glVertex3fv(v3);

    // Cara inferior (v4, v5, v1, v0) - Color: Cian
    glColor4ub(0, 255, 255, 255);
    glVertex3fv(v4); glVertex3fv(v5); glVertex3fv(v1);
    glVertex3fv(v1); glVertex3fv(v0); glVertex3fv(v4);

    glEnd();
}

static void draw_tetrahedron(const vec3& center, double size) {
    // Vértices del tetraedro
    static const GLfloat v0[3] = { 0.0f,  size,  0.0f };  // vértice superior
    static const GLfloat v1[3] = { -size, -size,  size };  // base izquierda
    static const GLfloat v2[3] = { size, -size,  size };  // base derecha
    static const GLfloat v3[3] = { 0.0f, -size, -size };  // base trasera

	set3dView(45);

    glBegin(GL_TRIANGLES);  // Dibujar el tetraedro con triángulos

    // Cara frontal (v0, v1, v2) - Color: Rojo
    glColor4ub(255, 0, 0, 255);
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2);

    // Cara derecha (v0, v2, v3) - Color: Verde
    glColor4ub(0, 255, 0, 255);
    glVertex3fv(v0); glVertex3fv(v2); glVertex3fv(v3);

    // Cara izquierda (v0, v3, v1) - Color: Azul
    glColor4ub(0, 0, 255, 255);
    glVertex3fv(v0); glVertex3fv(v3); glVertex3fv(v1);

    // Cara inferior (v1, v2, v3) - Color: Amarillo
    glColor4ub(255, 255, 0, 255);
    glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(v3);

    glEnd();
}

static void draw_octahedron(const vec3& center, double size) {
    // Vértices del octaedro en torno al centro especificado
    static const GLfloat v0[3] = { center.x, center.y + size, center.z };  // vértice superior
    static const GLfloat v1[3] = { center.x - size, center.y, center.z + size };  // base frontal izquierda
    static const GLfloat v2[3] = { center.x + size, center.y, center.z + size };  // base frontal derecha
    static const GLfloat v3[3] = { center.x + size, center.y, center.z - size };  // base trasera derecha
    static const GLfloat v4[3] = { center.x - size, center.y, center.z - size };  // base trasera izquierda
    static const GLfloat v5[3] = { center.x, center.y - size, center.z };  // vértice inferior

	set3dView(45);

    glBegin(GL_TRIANGLES);

    // Caras superiores (cuatro triángulos)
    // Cara 1: v0, v1, v2 (color rojo)
    glColor3f(1.0f, 0.0f, 0.0f);  // Color rojo
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2);

    // Cara 2: v0, v2, v3 (color verde)
    glColor3f(0.0f, 1.0f, 0.0f);  // Color verde
    glVertex3fv(v0); glVertex3fv(v2); glVertex3fv(v3);

    // Cara 3: v0, v3, v4 (color azul)
    glColor3f(0.0f, 0.0f, 1.0f);  // Color azul
    glVertex3fv(v0); glVertex3fv(v3); glVertex3fv(v4);

    // Cara 4: v0, v4, v1 (color amarillo)
    glColor3f(1.0f, 1.0f, 0.0f);  // Color amarillo
    glVertex3fv(v0); glVertex3fv(v4); glVertex3fv(v1);

    // Caras inferiores (cuatro triángulos)
    // Cara 5: v5, v2, v1 (color cian)
    glColor3f(0.0f, 1.0f, 1.0f);  // Color cian
    glVertex3fv(v5); glVertex3fv(v2); glVertex3fv(v1);

    // Cara 6: v5, v3, v2 (color magenta)
    glColor3f(1.0f, 0.0f, 1.0f);  // Color magenta
    glVertex3fv(v5); glVertex3fv(v3); glVertex3fv(v2);

    // Cara 7: v5, v4, v3 (color blanco)
    glColor3f(1.0f, 1.0f, 1.0f);  // Color blanco
    glVertex3fv(v5); glVertex3fv(v4); glVertex3fv(v3);

    // Cara 8: v5, v1, v4 (color gris)
    glColor3f(0.5f, 0.5f, 0.5f);  // Color gris
    glVertex3fv(v5); glVertex3fv(v1); glVertex3fv(v4);

    glEnd();
}
static int draw_fbx(const char *file) {
    set3dView(75);
    const struct aiScene* scene = aiImportFile(file, aiProcess_Triangulate);
    if (!scene) {
        fprintf(stderr, "Error en carregar el fitxer: %s\n", aiGetErrorString());
        return -1;
    }
    //printf("Numero de malles: %u\n", scene->mNumMeshes);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        printf("\nMalla %u:\n", i);
        printf(" Numero de vertexs: %u\n", mesh->mNumVertices);
        printf(" Numero de triangles: %u\n", mesh->mNumFaces);
        // Vèrtexs
        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            aiVector3D vertex = mesh->mVertices[v];
            printf(" Vertex %u: (%f, %f, %f)\n", v, vertex.x, vertex.y, vertex.z);
        }
        // Índexs de triangles (3 per triangle)
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {

            aiFace face = mesh->mFaces[f];
            printf(" Indexs triangle %u: ", f);

            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                printf("%u ", face.mIndices[j]);
            }

            printf("\n");
        }
        //pasar los arrays a openGL
        glBegin(GL_TRIANGLES);
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
			aiFace face = mesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
				aiVector3D vertex = mesh->mVertices[face.mIndices[j]];
				glVertex3f(vertex.x, vertex.y, vertex.z);
			}
		}
        glEnd();
    }
    aiReleaseImport(scene);
    return 0;
}


static void display_func() {
    // Limpiar el buffer de color y profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw_triangle(u8vec4(255, 0, 0, 255), vec3(0.0, 0.0, 0.0), 1.0);
	//draw_lines(u8vec4(0, 255, 0, 255), vec3(0.0, 0.0, 0.0), 1.0);
	//draw_cube(vec3(0.0, 0.0, 0.0), 1.0);
	//draw_prism(vec3(0.0, 0.0, 0.0), 2.0, 2.0, 1.0);
	//draw_pyramid(vec3(0.0, 0.0, 0.0), 1.0);
	//draw_tetrahedron(vec3(0.0, 0.0, 0.0), 1.0);
	//draw_octahedron(vec3(0.0, 0.0, 0.0), 1.0);
   
    draw_fbx("halo2.fbx");

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
