#include "DisplayFunc.h"

#include "ImporterFBX.h"
#include "BasicForms.h"
#include "Camera.h"
#include "Textures.h"


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

    if (!textureLoaded) {
        textures->LoadTexture(currentTextureFile.c_str());  // Cambia esta ruta a tu archivo de textura
        textureLoaded = true;  // Marcar que la textura ya ha sido cargada
    }

    // Dibujar la cuadrícula
    drawGrid(10.0f, 20);  // Puedes ajustar el tamaño y el número de divisiones según tus necesidades

    // Dibujar el contenido 3D
    importerFBX->draw_fbx(currentFBXFile.c_str());
    camera->CameraMovement();

    // Dibujar las formas si están activas, posicionadas para no solaparse
    //float separation = 5.0f;  // Distancia de separación entre objetos
    //float currentX = -1000000.0f;  // Posición X inicial

    if (cubeActive) {
        basicForms->draw_Cube(vec3(-10.0f, 0.0f, 0.0f), 1.0);  // Posición fija para el cubo
    }
    if (pyramidActive) {
        basicForms->draw_Pyramid(vec3(-5.0f, 0.0f, 0.0f), 1.0);  // Posición fija para la pirámide
    }
    if (sphereActive) {
        basicForms->draw_Sphere(vec3(5.0f, 0.0f, 0.0f), 1.0);  // Posición fija para la esfera
    }
    if (cylinderActive) {
        basicForms->draw_Cylindre(vec3(10.0f, 0.0f, 0.0f), 1.0, 2.0, 36);  // Posición fija para el cilindro
    }

    // Dibujar la textura cargada
    textures->DrawTexture();

    // Forzar el renderizado
    glFlush();
}
void DisplayFunc::drawGrid(float size, int divisions) {
    glColor3f(0.5f, 0.5f, 0.5f);  // Color de la cuadrícula (gris claro)
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
    // Desactivar todos los objetos antes de activar el seleccionado
   /* cubeActive = false;
    pyramidActive = false;
    sphereActive = false;
    cylinderActive = false;*/

    // Activar solo el objeto especificado
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
