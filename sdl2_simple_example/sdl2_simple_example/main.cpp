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

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64
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
    glEnable(GL_TEXTURE_2D);
	glClearColor(0.5, 0.5, 0.5, 1.0);
}

void set3dView(GLdouble fov) {
    // Configurar la matriz de proyección
    GLdouble yView = 50.0;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, 1.0, 0.1, 200.0);  // Perspectiva con campo de visión de 45 grados

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
static int draw_fbx(const char *file) {
    set3dView(75);
    const struct aiScene* scene = aiImportFile(file, aiProcess_Triangulate);
    if (!scene) {
        fprintf(stderr, "Error en carregar el fitxer: %s\n", aiGetErrorString());
        return -1;
    }
    //printf("Numero de malles: %u\n", scene->mNumMeshes);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        //actualizar la posicion de la camara
        aiMesh* mesh = scene->mMeshes[i];
        // Vèrtexs
        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            aiVector3D vertex = mesh->mVertices[v];
           
        }
        // Índexs de triangles (3 per triangle)
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {

            aiFace face = mesh->mFaces[f];
            

            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                
            }

        }
        //pasar los arrays a openGL
        glBegin(GL_TRIANGLES);
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
			aiFace face = mesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
				aiVector3D vertex = mesh->mVertices[face.mIndices[j]];
				glVertex3f(vertex.x, vertex.y, vertex.z);
				aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
				glTexCoord2f(uv.x, uv.y);
			}
		}
        glEnd();
    }
    aiReleaseImport(scene);
    return 0;
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
static void movimientoCamara() {
	//if mouse wheel up
    if (SDL_SCANCODE_UP) {
		glTranslatef(0.0, 0.0, 10.0);
    
	}
    if (SDL_SCANCODE_DOWN) {
		glTranslatef(0.0, 0.0, -10.0);
	}
    if (SDL_SCANCODE_LEFT) {
		glTranslatef(10.0, 0.0, 0.0);
	}
    if (SDL_SCANCODE_RIGHT) {
		glTranslatef(-10.0, 0.0, 0.0);
	}
}


static void display_func() {
    // Limpiar el buffer de color y profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//draw_cube(vec3(0.0, 0.0, 0.0), 1.0);
   movimientoCamara();
   // draw_fbx("halo2.fbx");
	//generate_textures();

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
