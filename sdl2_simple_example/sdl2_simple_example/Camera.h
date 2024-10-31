#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include "SDL2/SDL.h"


class Camera
{
public:
	Camera();
	~Camera();

	void Set3DView();

	void CameraMovement();

	void ResetCamera();

	const Uint8* state = SDL_GetKeyboardState(NULL);

};

