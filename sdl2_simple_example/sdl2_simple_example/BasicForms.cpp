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

	// Cara frontal (v0, v1, v2, v3) - Color: Verde
	glColor4ub(0, 255, 0, 255); // Verde
	glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2);  // Primer triángulo
	glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v0);  // Segundo triángulo

	// Cara derecha (v1, v5, v6, v2) - Color: Azul
	glColor4ub(0, 0, 255, 255); // Azul
	glVertex3fv(v1); glVertex3fv(v5); glVertex3fv(v6);  // Primer triángulo
	glVertex3fv(v6); glVertex3fv(v2); glVertex3fv(v1);  // Segundo triángulo

    // Cara trasera (v5, v4, v7, v6) - Color: Rojo
    glColor4ub(255, 0, 0, 255); // Rojo
    glVertex3fv(v5); glVertex3fv(v4); glVertex3fv(v7);  // Primer triángulo
    glVertex3fv(v7); glVertex3fv(v6); glVertex3fv(v5);  // Segundo triángulo

    // Cara izquierda (v4, v0, v3, v7) - Color: Amarillo
    glColor4ub(255, 255, 0, 255); // Amarillo
    glVertex3fv(v4); glVertex3fv(v0); glVertex3fv(v3);  // Primer triángulo
    glVertex3fv(v3); glVertex3fv(v7); glVertex3fv(v4);  // Segundo triángulo

    // Cara superior (v3, v2, v6, v7) - Color: Naranja
    glColor4ub(255, 165, 0, 255); // Naranja
    glVertex3fv(v3); glVertex3fv(v2); glVertex3fv(v6);  // Primer triángulo
    glVertex3fv(v6); glVertex3fv(v7); glVertex3fv(v3);  // Segundo triángulo

    // Cara inferior (v4, v5, v1, v0) - Color: Cian
    glColor4ub(0, 255, 255, 255); // Cian
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
    // Cara frontal (v0, v1, top) - Color: Rojo
    glColor4ub(255, 0, 0, 255);
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(top);
    // Cara derecha (v1, v2, top) - Color: Verde
    glColor4ub(0, 255, 0, 255);
    glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(top);
    // Cara trasera (v2, v3, top) - Color: Azul
    glColor4ub(0, 0, 255, 255);
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(top);
    // Cara izquierda (v3, v0, top) - Color: Amarillo
    glColor4ub(255, 255, 0, 255);
    glVertex3fv(v3); glVertex3fv(v0); glVertex3fv(top);
    // Base cuadrada (v0, v1, v2, v3) - Color: Cian
    glColor4ub(0, 255, 255, 255);
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2); // Primer triángulo
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v0); // Segundo triángulo
    glEnd();
}