#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "BasicForms.h"
#include "Input.h"

enum GizmoMode {
	NONE,
	TRANSLATE,
	SCALE,
	ROTATE
};

class Gizmo
{
public:
	Gizmo();
	~Gizmo();

	void drawTranslateGizmo();
	void drawScaleGizmo();
	void drawRotateGizmo();

	BasicForms* basicForms = new BasicForms;
};
extern Gizmo* gizmo;

