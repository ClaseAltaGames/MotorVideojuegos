#include "BasicForms.h"
#include <iostream>
#include <GL/glew.h>


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
        // V�rtices de la pir�mide

        
        break;
    }
}

void BasicForms::draw_Cube(const vec3& center, double size) {
	// V�rtices del cubo
	static const GLfloat v0[3] = { -1.0f, -1.0f,  1.0f };
	static const GLfloat v1[3] = { 1.0f, -1.0f,  1.0f };
	static const GLfloat v2[3] = { 1.0f,  1.0f,  1.0f };
	static const GLfloat v3[3] = { -1.0f,  1.0f,  1.0f };
	static const GLfloat v4[3] = { -1.0f, -1.0f, -1.0f };
	static const GLfloat v5[3] = { 1.0f, -1.0f, -1.0f };
	static const GLfloat v6[3] = { 1.0f,  1.0f, -1.0f };
	static const GLfloat v7[3] = { -1.0f,  1.0f, -1.0f };

	glBegin(GL_TRIANGLES);  // Dibujar el cubo con tri�ngulos

	glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2);  // Primer tri�ngulo
	glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v0);  // Segundo tri�ngulo

	glVertex3fv(v1); glVertex3fv(v5); glVertex3fv(v6);  // Primer tri�ngulo
	glVertex3fv(v6); glVertex3fv(v2); glVertex3fv(v1);  // Segundo tri�ngulo

    glVertex3fv(v5); glVertex3fv(v4); glVertex3fv(v7);  // Primer tri�ngulo
    glVertex3fv(v7); glVertex3fv(v6); glVertex3fv(v5);  // Segundo tri�ngulo

    glVertex3fv(v4); glVertex3fv(v0); glVertex3fv(v3);  // Primer tri�ngulo
    glVertex3fv(v3); glVertex3fv(v7); glVertex3fv(v4);  // Segundo tri�ngulo

    glVertex3fv(v3); glVertex3fv(v2); glVertex3fv(v6);  // Primer tri�ngulo
    glVertex3fv(v6); glVertex3fv(v7); glVertex3fv(v3);  // Segundo tri�ngulo

    glVertex3fv(v4); glVertex3fv(v5); glVertex3fv(v1);  // Primer tri�ngulo
    glVertex3fv(v1); glVertex3fv(v0); glVertex3fv(v4);  // Segundo tri�ngulo

    glEnd();
}

void BasicForms::draw_Pyramid(const vec3& center, double size) {
    // V�rtices de la pir�mide
    static const GLfloat v0[3] = { -1.0f, -1.0f,  1.0f }; // base inferior izquierda
    static const GLfloat v1[3] = { 1.0f, -1.0f,  1.0f }; // base inferior derecha
    static const GLfloat v2[3] = { 1.0f, -1.0f, -1.0f }; // base superior derecha
    static const GLfloat v3[3] = { -1.0f, -1.0f, -1.0f }; // base superior izquierda
    static const GLfloat top[3] = { 0.0f,  1.0f,  0.0f }; // v�rtice superior

    glBegin(GL_TRIANGLES);  // Dibujar la pir�mide con tri�ngulos

    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(top);
    glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(top);
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(top);
    glVertex3fv(v3); glVertex3fv(v0); glVertex3fv(top);
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2); // Primer tri�ngulo
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v0); // Segundo tri�ngulo
    glEnd();
}