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

void DisplayFunc::DisplayALL()
{
    // Limpiar el buffer de color y profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImporterFBX* importerFBX = new ImporterFBX;
    BasicForms* basicForms = new BasicForms;
    Camera* camera = new Camera;
    Textures* textures = new Textures;

    if (!textureLoaded) {
        textures->LoadTexture(TEXTURE_FILE);  // Cambia esta ruta a tu archivo de textura
        textureLoaded = true;  // Marcar que la textura ya ha sido cargada
    }

    // Dibujar el contenido 3D
    importerFBX->draw_fbx(currentFBXFile.c_str());
    camera->CameraMovement();

    // Dibujar las formas si están activas
    if (cubeActive == true) {
        basicForms->draw_Cube(vec3(0.0, 0.0, 0.0), 1.0);
    }
    if (pyramidActive == true) {
        basicForms->draw_Pyramid(vec3(0.0, 0.0, 0.0), 1.0);
    }
    if (sphereActive == true) {
        basicForms->draw_Sphere(vec3(0.0, 0.0, 0.0), 1.0);
    }
    if (cylinderActive == true) {
        basicForms->draw_Cylindre(vec3(0.0, 0.0, 0.0), 1.0, 2.0, 36);
    }

    // Dibujar la textura cargada
    textures->DrawTexture();

    // Forzar el renderizado
    glFlush();
}
