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
	bool Update();
	bool CleanUp();
	int Draw_FBX(const char* path);



};

