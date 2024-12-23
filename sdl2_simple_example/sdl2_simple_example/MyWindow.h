#pragma once
#include <string>
#include "ImporterFBX.h"
#include "DisplayFunc.h"
#include <cstdint>
#include "GL/gl.h"
#include "GL/glew.h"


struct SDL_Window;

class MyWindow {

	SDL_Window* _window = nullptr;
	void* _ctx = nullptr;

	int _width = 0;
	int _height = 0;

public:
	int width() const { return _width; }
	int height() const { return _height; }
	double aspectRatio() const { return static_cast<double>(_width) / _height; }

	MyWindow(const std::string& title, int w, int h);
	~MyWindow();

	void swapBuffers() const;


	ImporterFBX* importerFBX = new ImporterFBX();
	DisplayFunc* displayFunc = new DisplayFunc();

	string ObtenerInfoCPU() const;
	int GetScale() const;
	int scale;

	void setCurrentFBXFile(const std::string& fbxFile);
	char currentFBXFile;
	SDL_Window* getWindow() const { return _window; }
	

	
};
extern MyWindow* myWindow;