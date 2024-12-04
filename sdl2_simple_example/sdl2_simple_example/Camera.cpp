#include "Camera.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Input.h"
#include <iostream>
#include "MyWindow.h"


// Variables globales para la posici�n y orientaci�n de la c�mara
GLdouble cameraPosX = 5.0, cameraPosY = 5.0, cameraPosZ = 5.0;
GLdouble cameraDirX = 0.0, cameraDirY = 0.0, cameraDirZ = 0.0;
GLdouble cameraUpX = 0.0, cameraUpY = 1.0, cameraUpZ = 0.0;

// Variables para near y far
GLdouble nearPlane = 0.01;
GLdouble farPlane = 2000.0;

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Set3DView()
{
    const GLdouble fov = 45.0;

    // Configurar la matriz de proyecci�n
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Configurar perspectiva con el ratio de aspecto y los valores de near y far plane
	int windowWidth, windowHeight;
    SDL_GetWindowSize(myWindow->getWindow(), &windowWidth, &windowHeight);

    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    // Actualiza la proyecci�n con el aspecto correcto
    gluPerspective(fov, aspectRatio, std::max(static_cast<float>(nearPlane), 0.1f), std::min(static_cast<float>(farPlane), 100000.0f));

    // Configuraci�n de la c�mara en la matriz de modelo-vista
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Configurar la c�mara mirando hacia el objetivo
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ,
        cameraDirX, cameraDirY, cameraDirZ,
        cameraUpX, cameraUpY, cameraUpZ);
}

void Camera::CameraMovement()
{
    Set3DView();
    ResetCamera();

    // Movimiento con el mouse si el bot�n derecho est� presionado
    if (input->GetMouseButton(RIGHT_BUTTON) == KEY_DOWN) {
        OrbitWorld();
    }

	// Movimiento con el mouse si el bot�n izquierdo est� presionado
	if (input->GetMouseButton(LEFT_BUTTON) == KEY_DOWN && state[SDL_SCANCODE_LALT]) {
		OrbitObject();
	}

    // Rotaci�n suave con teclas de flecha
    if (state[SDL_SCANCODE_LEFT]) {
        cameraDirX = cameraPosX + cos(rotationSpeed) * (cameraDirX - cameraPosX) - sin(rotationSpeed) * (cameraDirZ - cameraPosZ);
        cameraDirZ = cameraPosZ + sin(rotationSpeed) * (cameraDirX - cameraPosX) + cos(rotationSpeed) * (cameraDirZ - cameraPosZ);
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        cameraDirX = cameraPosX + cos(-rotationSpeed) * (cameraDirX - cameraPosX) - sin(-rotationSpeed) * (cameraDirZ - cameraPosZ);
        cameraDirZ = cameraPosZ + sin(-rotationSpeed) * (cameraDirX - cameraPosX) + cos(-rotationSpeed) * (cameraDirZ - cameraPosZ);
    }
    if (state[SDL_SCANCODE_UP]) {
        cameraPosY += moveSpeed;
    }
    if (state[SDL_SCANCODE_DOWN]) {
        cameraPosY -= moveSpeed;
    }
}

void Camera::ResetCamera()
{
    if (state[SDL_SCANCODE_F])
    {
        cameraPosX = 5.0;
        cameraPosY = 5.0;
        cameraPosZ = 5.0;
        cameraDirX = 0.0;
        cameraDirY = 0.0;
        cameraDirZ = 0.0;
        cameraUpX = 0.0;
        cameraUpY = 1.0;
        cameraUpZ = 0.0;
        nearPlane = 0.01;
        farPlane = 2000.0;
    }
}

void Camera::ZoomIN()
{
	cameraPosX += 0.25;
    cameraPosY += 0.25;
	cameraPosZ += 0.25;
}

void Camera::ZoomOUT()
{
	cameraPosX -= 0.25;
    cameraPosY -= 0.25;
	cameraPosZ -= 0.25;
}

void Camera::OrbitWorld()
{
    // Vector direcci�n de la c�mara en el plano XZ (para avanzar/retroceder)
    GLdouble forwardX = cameraDirX - cameraPosX;
    GLdouble forwardZ = cameraDirZ - cameraPosZ;
    double magnitude = sqrt(forwardX * forwardX + forwardZ * forwardZ);

    // Normalizar la direcci�n
    forwardX /= magnitude;
    forwardZ /= magnitude;

    // Vector lateral (perpendicular al vector de direcci�n)
    GLdouble strafeX = -forwardZ;
    GLdouble strafeZ = forwardX;


    if (state[SDL_SCANCODE_LSHIFT]) {
        moveSpeed *= 2.0f;
        rotationSpeed *= 2.0f;
    }

    // Movimiento hacia adelante y atr�s (W y S)
    if (state[SDL_SCANCODE_W]) {
        cameraPosX += forwardX * moveSpeed;
        cameraPosZ += forwardZ * moveSpeed;
        cameraDirX += forwardX * moveSpeed;
        cameraDirZ += forwardZ * moveSpeed;
    }
    if (state[SDL_SCANCODE_S]) {
        cameraPosX -= forwardX * moveSpeed;
        cameraPosZ -= forwardZ * moveSpeed;
        cameraDirX -= forwardX * moveSpeed;
        cameraDirZ -= forwardZ * moveSpeed;
    }

	// Movimiento lateral (A y D)
    if (state[SDL_SCANCODE_A]) {
        cameraPosX -= strafeX * moveSpeed;
        cameraPosZ -= strafeZ * moveSpeed;
    }
    if (state[SDL_SCANCODE_D]) {
        cameraPosX += strafeX * moveSpeed;
        cameraPosZ += strafeZ * moveSpeed;
    }
}

void Camera::OrbitObject()
{
	//haz que de vueltas alrededor del objeto automaticamente
	cameraPosX = 0.0 + 5.0 * cos(SDL_GetTicks() / 1000.0);
	cameraPosZ = 0.0 + 5.0 * sin(SDL_GetTicks() / 1000.0);
	cameraPosY = 0.0 + 5.0 * sin(SDL_GetTicks() / 1000.0);

	cameraDirX = 0.0;
	cameraDirY = 0.0;
	cameraDirZ = 0.0;

	cameraUpX = 0.0;
	cameraUpY = 1.0;
	cameraUpZ = 0.0;

	nearPlane = 0.01;
	farPlane = 2000.0;
}