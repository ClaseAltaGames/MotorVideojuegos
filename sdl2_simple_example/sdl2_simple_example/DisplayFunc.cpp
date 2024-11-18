#include "DisplayFunc.h"

#include "ImporterFBX.h"
#include "BasicForms.h"
#include "Camera.h"
#include "Textures.h"
#include "Input.h"
#include "SDL2/SDL.h"

DisplayFunc::DisplayFunc()
{
}

DisplayFunc::~DisplayFunc()
{
}

void DisplayFunc::DisplayALL() {
    // Limpiar el buffer de color y profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImporterFBX* importerFBX = new ImporterFBX;
    BasicForms* basicForms = new BasicForms;
    Camera* camera = new Camera;
    Textures* textures = new Textures;
	Gizmo* gizmo = new Gizmo;

    if (!textureLoaded) {
        textures->LoadTexture(currentTextureFile.c_str());  // Cambia esta ruta a tu archivo de textura
        textureLoaded = true;  // Marcar que la textura ya ha sido cargada
    }

    if (camera->state[SDL_SCANCODE_K])
    {
		importerFBX->setPosition(5.0f, 0.0f, 0.0f);
		importerFBX->setRotation(5.0f, 5.0f, 5.0f);
		importerFBX->setScale(2.0f, 2.0f, 2.0f);
    }
    // Dibujar la cuadrícula
    drawGrid(10.0f, 20);

    // Dibujar el contenido 3D
    importerFBX->draw_fbx(currentFBXFile.c_str());
    camera->CameraMovement();



    if (cubeActive) {
        basicForms->draw_Cube(vec3(-10.0f, 0.0f, 0.0f), 1.0);  // El -10 es para que el cubo no se superponga con la pirámide (ajuste de la posicion)
    }
    if (pyramidActive) {
        basicForms->draw_Pyramid(vec3(-5.0f, 0.0f, 0.0f), 1.0);  //Lo mismo
    }
    if (sphereActive) {
        basicForms->draw_Sphere(vec3(5.0f, 0.0f, 0.0f), 1.0);  // Lo mismo
    }
    if (cylinderActive) {
        basicForms->draw_Cylindre(vec3(10.0f, 0.0f, 0.0f), 1.0, 2.0, 36);  // Lo mismo
    }

    // Dibujar el Gizmo según el modo actual
    switch (gizmo->currentGizmoMode) {
    case TRANSLATE:
        gizmo->drawTranslateGizmo();
        break;
    case SCALE:
        gizmo->drawScaleGizmo();
        break;
    case ROTATE:
        gizmo->drawRotateGizmo();
        break;
    }


    // Dibujar la textura cargada
    textures->DrawTexture();

    // Forzar el renderizado
    glFlush();
}
void DisplayFunc::drawGrid(float size, int divisions) {
    glColor3f(0.5f, 0.5f, 0.5f);  // Color de la cuadrícula
    glLineWidth(1.0f);             // Ancho de línea

    glBegin(GL_LINES);
    for (int i = -divisions; i <= divisions; i++) {
        float pos = i * (size / divisions);

        // Línea horizontal
        glVertex3f(-size, 0.0f, pos);
        glVertex3f(size, 0.0f, pos);

        // Línea vertical
        glVertex3f(pos, 0.0f, -size);
        glVertex3f(pos, 0.0f, size);
    }
    glEnd();
}
void DisplayFunc::setActiveObject(const std::string& objectName) {

    // Activar el objeto especificado
    if (objectName == "cube") {
        cubeActive = true;
    }
    else if (objectName == "pyramid") {
        pyramidActive = true;
    }
    else if (objectName == "sphere") {
        sphereActive = true;
    }
    else if (objectName == "cylinder") {
        cylinderActive = true;
    }
}
