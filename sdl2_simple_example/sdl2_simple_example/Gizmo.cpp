#include "Gizmo.h"

Gizmo::Gizmo()
{
}

Gizmo::~Gizmo()
{
}

void Gizmo::drawTranslateGizmo() {

    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo para X
    basicForms->drawArrow(glm::vec3(1.0f, 0.0f, 0.0f));

    glColor3f(0.0f, 1.0f, 0.0f); // Verde para Y
    basicForms->drawArrow(glm::vec3(0.0f, 1.0f, 0.0f));

    glColor3f(0.0f, 0.0f, 1.0f); // Azul para Z
    basicForms->drawArrow(glm::vec3(0.0f, 0.0f, 1.0f));
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

