#pragma once
#include <glm/glm.hpp>

using vec3 = glm::dvec3;

enum Formas {
	CUBO,
	PIRAMIDE,
	VACIO
};

class BasicForms
{
public:
	BasicForms();
	~BasicForms();

	void cambiarFormas(Formas forma);

	void draw_Cube(const vec3& center, double size);

	void draw_Pyramid(const vec3& center, double size);	

	void draw_Cylindre(const vec3& center, double radius, double height, int segments);

};

