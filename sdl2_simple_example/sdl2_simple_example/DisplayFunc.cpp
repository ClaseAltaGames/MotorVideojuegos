#include "DisplayFunc.h"

#include "ImporterFBX.h"
#include "BasicForms.h"
#include "Camera.h"


#define FBX_FILE "Assets/BakerHouse.fbx"

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

	importerFBX->draw_fbx(FBX_FILE);
	camera->CameraMovement();
	basicForms->cambiarFormas(Formas::CUBO);

	// Forzar el renderizado
	glFlush();

}
