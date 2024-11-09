#pragma once
#include <string>

using namespace std;

#define FBX_FILE "Assets/BakerHouse.fbx"
#define TEXTURE_FILE "Assets/Baker_house.dds"

class DisplayFunc
{
public:

	DisplayFunc();
	~DisplayFunc();

	void DisplayALL();
	void drawGrid(float size, int divisions);
	void setActiveObject(const std::string& objectName);

	bool cubeActive = false;
	bool pyramidActive = false;
	bool sphereActive = false;
	bool cylinderActive = false;
	bool textureLoaded = false;

	string currentFBXFile = FBX_FILE;
	string currentTextureFile = TEXTURE_FILE;

};

