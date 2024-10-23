#pragma once
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include <iostream>

class ImporterFBX
{
public:
	ImporterFBX();
	~ImporterFBX();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	// Función para cargar el archivo FBX y extraer los datos
	int Load_FBX(const char* file, std::vector<std::vector<aiVector3D>>& vertices, std::vector<std::vector<aiVector3D>>& uvs);

	// Función para dibujar las mallas cargadas con OpenGL
	void Draw_FBX(const std::vector<std::vector<aiVector3D>>& vertices, const std::vector<std::vector<aiVector3D>>& uvs);
};
