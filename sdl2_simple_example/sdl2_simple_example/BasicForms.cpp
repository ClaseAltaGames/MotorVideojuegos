#include "BasicForms.h"
#include <iostream>
#include <GL/glew.h>
#include <cmath>

#define M_PI 3.14159265358979323846

BasicForms::BasicForms()
{
}

BasicForms::~BasicForms()
{
}

void BasicForms::cambiarFormas(Formas forma)
{
    switch (forma)
    {
    case CUBO:
        draw_Cube(vec3(0.0, 0.0, 0.0), 1.0);
        break;
    case PIRAMIDE:
        // Vértices de la pirámide

        
        break;
    }
}

void BasicForms::draw_Cube(const vec3& center, double size) {
	// Vértices del cubo
	static const GLfloat v0[3] = { -1.0f, -1.0f,  1.0f };
	static const GLfloat v1[3] = { 1.0f, -1.0f,  1.0f };
	static const GLfloat v2[3] = { 1.0f,  1.0f,  1.0f };
	static const GLfloat v3[3] = { -1.0f,  1.0f,  1.0f };
	static const GLfloat v4[3] = { -1.0f, -1.0f, -1.0f };
	static const GLfloat v5[3] = { 1.0f, -1.0f, -1.0f };
	static const GLfloat v6[3] = { 1.0f,  1.0f, -1.0f };
	static const GLfloat v7[3] = { -1.0f,  1.0f, -1.0f };

	glBegin(GL_TRIANGLES);  // Dibujar el cubo con triángulos

	glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2);  // Primer triángulo
	glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v0);  // Segundo triángulo

	glVertex3fv(v1); glVertex3fv(v5); glVertex3fv(v6);  // Primer triángulo
	glVertex3fv(v6); glVertex3fv(v2); glVertex3fv(v1);  // Segundo triángulo

    glVertex3fv(v5); glVertex3fv(v4); glVertex3fv(v7);  // Primer triángulo
    glVertex3fv(v7); glVertex3fv(v6); glVertex3fv(v5);  // Segundo triángulo

    glVertex3fv(v4); glVertex3fv(v0); glVertex3fv(v3);  // Primer triángulo
    glVertex3fv(v3); glVertex3fv(v7); glVertex3fv(v4);  // Segundo triángulo

    glVertex3fv(v3); glVertex3fv(v2); glVertex3fv(v6);  // Primer triángulo
    glVertex3fv(v6); glVertex3fv(v7); glVertex3fv(v3);  // Segundo triángulo

    glVertex3fv(v4); glVertex3fv(v5); glVertex3fv(v1);  // Primer triángulo
    glVertex3fv(v1); glVertex3fv(v0); glVertex3fv(v4);  // Segundo triángulo

    glEnd();
}

void BasicForms::draw_Pyramid(const vec3& center, double size) {
    // Vértices de la pirámide
    static const GLfloat v0[3] = { -1.0f, -1.0f,  1.0f }; // base inferior izquierda
    static const GLfloat v1[3] = { 1.0f, -1.0f,  1.0f }; // base inferior derecha
    static const GLfloat v2[3] = { 1.0f, -1.0f, -1.0f }; // base superior derecha
    static const GLfloat v3[3] = { -1.0f, -1.0f, -1.0f }; // base superior izquierda
    static const GLfloat top[3] = { 0.0f,  1.0f,  0.0f }; // vértice superior

    glBegin(GL_TRIANGLES);  // Dibujar la pirámide con triángulos

    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(top);
    glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(top);
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(top);
    glVertex3fv(v3); glVertex3fv(v0); glVertex3fv(top);
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2); // Primer triángulo
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v0); // Segundo triángulo
    glEnd();
}

void BasicForms::draw_Cylindre(const vec3& center, double radius, double height, int segments) {
    double halfHeight = height / 2.0;

    // Dibujar tapa superior
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(center.x, center.y + halfHeight, center.z); // Centro de la tapa superior
    for (int i = 0; i <= segments; ++i) {
        double theta = 2.0 * M_PI * i / segments;
        double x = radius * cos(theta);
        double z = radius * sin(theta);
        glVertex3f(center.x + x, center.y + halfHeight, center.z + z);
    }
    glEnd();

    // Dibujar tapa inferior
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(center.x, center.y - halfHeight, center.z); // Centro de la tapa inferior
    for (int i = 0; i <= segments; ++i) {
        double theta = 2.0 * M_PI * i / segments;
        double x = radius * cos(theta);
        double z = radius * sin(theta);
        glVertex3f(center.x + x, center.y - halfHeight, center.z + z);
    }
    glEnd();

    // Dibujar pared lateral
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        double theta = 2.0 * M_PI * i / segments;
        double x = radius * cos(theta);
        double z = radius * sin(theta);

        glVertex3f(center.x + x, center.y - halfHeight, center.z + z); // Punto inferior
        glVertex3f(center.x + x, center.y + halfHeight, center.z + z); // Punto superior
    }
    glEnd();
}