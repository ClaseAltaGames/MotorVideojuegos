#include "Gizmo.h"

Gizmo::Gizmo()
{
}

Gizmo::~Gizmo()
{
}

void Gizmo::handleInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_w: currentGizmoMode = TRANSLATE; break;
        case SDLK_e: currentGizmoMode = SCALE; break;
        case SDLK_r: currentGizmoMode = ROTATE; break;
        }
    }
}
void Gizmo::drawTranslateGizmo() {
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo para X
    basicForms->drawArrow(glm::vec3(1.0f, 0.0f, 0.0f));

    glColor3f(0.0f, 1.0f, 0.0f); // Verde para Y
    basicForms->drawArrow(glm::vec3(0.0f, 1.0f, 0.0f));

    glColor3f(0.0f, 0.0f, 1.0f); // Azul para Z
    basicForms->drawArrow(glm::vec3(0.0f, 0.0f, 1.0f));
}
void Gizmo::drawScaleGizmo() {
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo para X
    basicForms->drawCube(glm::vec3(1.0f, 0.0f, 0.0f));

    glColor3f(0.0f, 1.0f, 0.0f); // Verde para Y
    basicForms->drawCube(glm::vec3(0.0f, 1.0f, 0.0f));

    glColor3f(0.0f, 0.0f, 1.0f); // Azul para Z
    basicForms->drawCube(glm::vec3(0.0f, 0.0f, 1.0f));
}
void Gizmo::drawRotateGizmo() {
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo para X
    basicForms->drawCircle(glm::vec3(1.0f, 0.0f, 0.0f));

    glColor3f(0.0f, 1.0f, 0.0f); // Verde para Y
    basicForms->drawCircle(glm::vec3(0.0f, 1.0f, 0.0f));

    glColor3f(0.0f, 0.0f, 1.0f); // Azul para Z
    basicForms->drawCircle(glm::vec3(0.0f, 0.0f, 1.0f));
}

