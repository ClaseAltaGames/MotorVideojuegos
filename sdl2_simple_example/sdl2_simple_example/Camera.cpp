#include "Camera.h"

// Variables globales para la posición y orientación de la cámara
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

    // Configurar la matriz de proyección
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, 1.0, nearPlane, farPlane);

    // Configuración de la cámara
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ,
        cameraDirX, cameraDirY, cameraDirZ,
        cameraUpX, cameraUpY, cameraUpZ);
}

void Camera::CameraMovement()
{
    Set3DView();
	ResetCamera();
    // Control de velocidad de movimiento y rotación
    const double moveSpeed = 2.0;
    const double rotationSpeed = 0.05;

    // Actualizar posición de la cámara con teclas de dirección
    if (state[SDL_SCANCODE_W]) {
        cameraPosX += moveSpeed * (cameraDirX - cameraPosX) * 0.1; // Ajustar el movimiento
        cameraPosZ += moveSpeed * (cameraDirZ - cameraPosZ) * 0.1; // Ajustar el movimiento
    }
    if (state[SDL_SCANCODE_S]) {
        cameraPosX -= moveSpeed * (cameraDirX - cameraPosX) * 0.1; // Ajustar el movimiento
        cameraPosZ -= moveSpeed * (cameraDirZ - cameraPosZ) * 0.1; // Ajustar el movimiento
    }
    if (state[SDL_SCANCODE_A]) {
        cameraPosX -= moveSpeed * (cameraDirZ - cameraPosZ) * 0.1; // Ajustar el movimiento
        cameraPosZ += moveSpeed * (cameraDirX - cameraPosX) * 0.1; // Ajustar el movimiento
    }
    if (state[SDL_SCANCODE_D]) {
        cameraPosX += moveSpeed * (cameraDirZ - cameraPosZ) * 0.1; // Ajustar el movimiento
        cameraPosZ -= moveSpeed * (cameraDirX - cameraPosX) * 0.1; // Ajustar el movimiento
    }

    // Rotar vista con teclas de flecha
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

    // Ajustar near y far planes según la posición de la cámara
    nearPlane = 0.01; // Asegurarse de que el plano cercano no sea demasiado pequeño
    farPlane = 2000.0; // Ajustar el plano lejano
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


