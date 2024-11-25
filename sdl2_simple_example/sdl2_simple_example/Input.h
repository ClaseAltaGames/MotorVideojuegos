#ifndef __INPUT_H__
#define __INPUT_H__


#define NUM_MOUSE_BUTTONS 5

struct SDL_Rect;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};
enum MouseButton {
	LEFT_BUTTON = 0,
	MIDDLE_BUTTON,
	RIGHT_BUTTON
};

class Input 
{

public:

	Input(bool startEnabled = true);

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);
	KeyState GetMouseButton(MouseButton button);
	void SetMouseButton(MouseButton button, KeyState state);
    


	bool windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;
};
extern Input* input;

#endif // __INPUT_H__