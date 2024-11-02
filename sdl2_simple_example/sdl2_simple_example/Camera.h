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

};

