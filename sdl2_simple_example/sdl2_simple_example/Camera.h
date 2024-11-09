#pragma once

class Camera
{
public:
	Camera();
	~Camera();

	void Set3DView();

	void CameraMovement();

	void ResetCamera();

	void ZoomIN();

	void ZoomOUT();

	void OrbitWorld();

	void OrbitObject();

    double moveSpeed = 0.1;       // Velocidad de avance y desplazamiento lateral
    double rotationSpeed = 0.05;  // Cantidad de rotación




};

