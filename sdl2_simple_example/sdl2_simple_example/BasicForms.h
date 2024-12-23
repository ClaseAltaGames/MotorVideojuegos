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

	void draw_Cube(const vec3& center, double size);

	void draw_Pyramid(const vec3& center, double size);	

	void draw_Sphere(const vec3& center, double radius);

	void draw_Cylindre(const vec3& center, double radius, double height, int segments);

	void drawArrow(const glm::vec3& direction);

	void drawCube(const glm::vec3& position);

	void drawCircle(const glm::vec3& axis);
};
extern BasicForms* basicForms;
