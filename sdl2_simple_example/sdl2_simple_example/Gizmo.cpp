#include "Gizmo.h"

Gizmo::Gizmo()
{
}

Gizmo::~Gizmo()
{
}

void Gizmo::drawTranslateGizmo() {
    glDisable(GL_DEPTH_TEST);

    // Dibujar flecha en X (Rojo)
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(1.0f, 0.0f, 0.0f);  // Trasladar en X
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotar 90° sobre Y
    basicForms->drawArrow(glm::vec3(0.0f, 0.0f, 0.0f));
    glPopMatrix();

    // Dibujar flecha en Y (Verde)
    glPushMatrix();
    glColor3f(0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 1.0f, 0.0f);  // Trasladar en Y
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotar -90° sobre X
    basicForms->drawArrow(glm::vec3(0.0f, 0.0f, 0.0f));
    glPopMatrix();

    // Dibujar flecha en Z (Azul)
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, 1.0f);  // Trasladar en Z
    basicForms->drawArrow(glm::vec3(0.0f, 0.0f, 0.0f));
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
}
void Gizmo::drawScaleGizmo() {
    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo para X
    basicForms->drawCube(glm::vec3(1.0f, 0.0f, 0.0f));

    glColor3f(0.0f, 1.0f, 0.0f); // Verde para Y
    basicForms->drawCube(glm::vec3(0.0f, 1.0f, 0.0f));

    glColor3f(0.0f, 0.0f, 1.0f); // Azul para Z
    basicForms->drawCube(glm::vec3(0.0f, 0.0f, 1.0f));
    glEnable(GL_DEPTH_TEST);
}
void Gizmo::drawRotateGizmo() {
    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo para X
    basicForms->drawCircle(glm::vec3(1.0f, 0.0f, 0.0f));

    glColor3f(0.0f, 1.0f, 0.0f); // Verde para Y
    basicForms->drawCircle(glm::vec3(0.0f, 1.0f, 0.0f));

    glColor3f(0.0f, 0.0f, 1.0f); // Azul para Z
    basicForms->drawCircle(glm::vec3(0.0f, 0.0f, 1.0f));
    glEnable(GL_DEPTH_TEST);
}

