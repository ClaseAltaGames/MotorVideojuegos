#include "BasicForms.h"
#include <iostream>
#include <cmath>
#include <GL/glew.h>

#define M_PI 3.14159265358979323846


BasicForms::BasicForms()
{
}

BasicForms::~BasicForms()
{
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


void BasicForms::draw_Sphere(const vec3& center, double radius) {
    const int numLatitudeBands = 20;  // Número de bandas en la latitud
    const int numLongitudeBands = 20; // Número de bandas en la longitud

    for (int lat = 0; lat <= numLatitudeBands; ++lat) {
        double theta1 = lat * M_PI / numLatitudeBands;
        double theta2 = (lat + 1) * M_PI / numLatitudeBands;

        double sinTheta1 = sin(theta1);
        double cosTheta1 = cos(theta1);
        double sinTheta2 = sin(theta2);
        double cosTheta2 = cos(theta2);

        glBegin(GL_TRIANGLE_STRIP);
        for (int lon = 0; lon <= numLongitudeBands; ++lon) {
            double phi = lon * 2 * M_PI / numLongitudeBands;
            double sinPhi = sin(phi);
            double cosPhi = cos(phi);

            // Primer vértice del triángulo
            GLfloat x1 = cosPhi * sinTheta1;
            GLfloat y1 = cosTheta1;
            GLfloat z1 = sinPhi * sinTheta1;

            // Segundo vértice del triángulo
            GLfloat x2 = cosPhi * sinTheta2;
            GLfloat y2 = cosTheta2;
            GLfloat z2 = sinPhi * sinTheta2;

            glVertex3f(center.x + radius * x1, center.y + radius * y1, center.z + radius * z1);
            glVertex3f(center.x + radius * x2, center.y + radius * y2, center.z + radius * z2);
        }
        glEnd();
    }
}
