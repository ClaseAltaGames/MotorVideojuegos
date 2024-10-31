#include "DisplayFunc.h"

#include "ImporterFBX.h"
#include "BasicForms.h"
#include "Camera.h"
#include "Textures.h"


#define FBX_FILE "Assets/BakerHouse.fbx"
#define TEXTURE_FILE "Assets/Baker_house.png"

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

	importerFBX->draw_fbx(FBX_FILE);
	camera->CameraMovement();
	if (cubeActive == true) {

		basicForms->draw_Cube(vec3(0.0, 0.0, 0.0), 1.0);
	}
	if (pyramidActive == true) {
		basicForms->draw_Pyramid(vec3(0.0, 0.0, 0.0), 1.0);
	}
	textures->DrawTextures(TEXTURE_FILE);


	// Forzar el renderizado
	glFlush();

}
