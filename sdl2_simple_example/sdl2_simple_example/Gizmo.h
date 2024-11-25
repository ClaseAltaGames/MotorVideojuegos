#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "BasicForms.h"
#include "Input.h"

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
	GizmoMode currentGizmoMode;  // Modo inicial
	void changeGizmoMode();
	void drawTranslateGizmo();
	void drawScaleGizmo();
	void drawRotateGizmo();
	void draw();


	BasicForms* basicForms = new BasicForms;
};


