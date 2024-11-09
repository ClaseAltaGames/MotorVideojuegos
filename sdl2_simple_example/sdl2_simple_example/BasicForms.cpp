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
    GLfloat v0[3] = { center.x - 1.0f, center.y - 1.0f, center.z + 1.0f };
    GLfloat v1[3] = { center.x + 1.0f, center.y - 1.0f, center.z + 1.0f };
    GLfloat v2[3] = { center.x + 1.0f, center.y + 1.0f, center.z + 1.0f };
    GLfloat v3[3] = { center.x - 1.0f, center.y + 1.0f, center.z + 1.0f };
    GLfloat v4[3] = { center.x - 1.0f, center.y - 1.0f, center.z - 1.0f };
    GLfloat v5[3] = { center.x + 1.0f, center.y - 1.0f, center.z - 1.0f };
    GLfloat v6[3] = { center.x + 1.0f, center.y + 1.0f, center.z - 1.0f };
    GLfloat v7[3] = { center.x - 1.0f, center.y + 1.0f, center.z - 1.0f };

    glBegin(GL_TRIANGLES);
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2);
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v0);

    glVertex3fv(v1); glVertex3fv(v5); glVertex3fv(v6);
    glVertex3fv(v6); glVertex3fv(v2); glVertex3fv(v1);

    glVertex3fv(v5); glVertex3fv(v4); glVertex3fv(v7);
    glVertex3fv(v7); glVertex3fv(v6); glVertex3fv(v5);

    glVertex3fv(v4); glVertex3fv(v0); glVertex3fv(v3);
    glVertex3fv(v3); glVertex3fv(v7); glVertex3fv(v4);

    glVertex3fv(v3); glVertex3fv(v2); glVertex3fv(v6);
    glVertex3fv(v6); glVertex3fv(v7); glVertex3fv(v3);

    glVertex3fv(v4); glVertex3fv(v5); glVertex3fv(v1);
    glVertex3fv(v1); glVertex3fv(v0); glVertex3fv(v4);

    glEnd();
}

void BasicForms::draw_Pyramid(const vec3& center, double size) {
    GLfloat v0[3] = { center.x - 1.0f, center.y - 1.0f, center.z + 1.0f };
    GLfloat v1[3] = { center.x + 1.0f, center.y - 1.0f, center.z + 1.0f };
    GLfloat v2[3] = { center.x + 1.0f, center.y - 1.0f, center.z - 1.0f };
    GLfloat v3[3] = { center.x - 1.0f, center.y - 1.0f, center.z - 1.0f };
    GLfloat top[3] = { center.x, center.y + 1.0f, center.z };

    glBegin(GL_TRIANGLES);
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(top);
    glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(top);
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(top);
    glVertex3fv(v3); glVertex3fv(v0); glVertex3fv(top);
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2);
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v0);
    glEnd();
}


void BasicForms::draw_Sphere(const vec3& center, double radius) {
    const int numLatitudeBands = 20;
    const int numLongitudeBands = 20;

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

            GLfloat x1 = cosPhi * sinTheta1;
            GLfloat y1 = cosTheta1;
            GLfloat z1 = sinPhi * sinTheta1;

            GLfloat x2 = cosPhi * sinTheta2;
            GLfloat y2 = cosTheta2;
            GLfloat z2 = sinPhi * sinTheta2;

            glVertex3f(center.x + radius * x1, center.y + radius * y1, center.z + radius * z1);
            glVertex3f(center.x + radius * x2, center.y + radius * y2, center.z + radius * z2);
        }
        glEnd();
    }
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
