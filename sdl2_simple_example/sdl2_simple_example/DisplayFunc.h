#pragma once
#include <string>

using namespace std;

#define FBX_FILE "Assets/BakerHouse.fbx"
#define TEXTURE_FILE "Assets/Baker_house.png"

class DisplayFunc
{
public:

	DisplayFunc();
	~DisplayFunc();

	void DisplayALL();

	bool cubeActive = false;
	bool pyramidActive = false;

	string currentFBXFile = FBX_FILE;

};

