#pragma once
#include "SDL2/SDL.h"


class Camera
{
public:
	Camera();
	~Camera();

	void Set3DView();

	void CameraMovement();

	void ResetCamera();

	void ZoomIN();

	void ZoomOUT();

	void OrbitWorld();

	void OrbitObject();

    double moveSpeed = 0.1;       
    double rotationSpeed = 0.05;  

	const Uint8* state = SDL_GetKeyboardState(NULL);
};

