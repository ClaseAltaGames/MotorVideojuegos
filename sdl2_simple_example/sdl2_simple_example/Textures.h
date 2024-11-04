#pragma once
#include <GL/glew.h>
#include <iostream>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include "Component.h"



class Textures : public Component
{
public:
	Textures();
	~Textures();
	void DrawTextures(const char* filepath);
	void BindTexture();

	GLuint textureID;

};

