#pragma once
#include <string>

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

};