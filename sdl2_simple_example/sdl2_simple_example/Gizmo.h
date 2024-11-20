#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "BasicForms.h"

enum GizmoMode {
	TRANSLATE,
	SCALE,
	ROTATE
};

class Gizmo
{
public:
	Gizmo();
	~Gizmo();

	double moveSpeed = 0.1;
	double rotationSpeed = 0.05;
	GizmoMode currentGizmoMode = SCALE;  // Modo inicial
	void handleInput(SDL_Event& event);
	void drawTranslateGizmo();
	void drawScaleGizmo();
	void drawRotateGizmo();

	BasicForms* basicForms = new BasicForms;
};


