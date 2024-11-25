#include "Gizmo.h"

Gizmo::Gizmo()
{
}

Gizmo::~Gizmo()
{
}

void Gizmo::changeGizmoMode() {
    // Cambiar el modo de Gizmo al detectar una pulsación inicial
    if (input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
        currentGizmoMode = TRANSLATE;
    }
    if (input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
        currentGizmoMode = ROTATE;
    }
    if (input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
        currentGizmoMode = SCALE;
    }
}

void Gizmo::draw() {
    // Dibujar el Gizmo según el modo actual
    switch (currentGizmoMode) {
    case TRANSLATE:
        ///////////////////////////////////////////////////
        //corregir el porque no entra aqui el proximo dia//
        ///////////////////////////////////////////////////
        printf("I'm here");
        drawTranslateGizmo();
        break;
    case ROTATE:
        drawRotateGizmo();
        break;
    case SCALE:
        drawScaleGizmo();
        break;
    case NONE:
        break; // No se dibuja nada si no hay un modo válido
    default:
        break; // No se dibuja nada si no hay un modo válido
    }
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

