#ifndef __COMPONENT_H__
#define __COMPONENT_H__


class GameObject;

enum class typeComponent {
	Error = -1,
	Transform,
	Mesh,
	Material,
	Camera,
	Canvas,
	UI,
};

class Component {
public:
	Component() {};

	typeComponent type;
	bool active;
	GameObject* parent;

	virtual void Enable() {};
	virtual void Update() {};
	virtual void Disable() {};

};

#endif // __COMPONENT_H__
