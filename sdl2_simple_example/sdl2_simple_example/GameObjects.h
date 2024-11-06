#pragma once
#include "vector"
#include "string"
#include "Component.h"


using namespace std;

class GameObjects : public Component
{
public:
	GameObjects();
	~GameObjects();
	
	void Update() override;

	//Component* CreateComponent(ComponentType);

	vector<Component*> components;
	string name;
	
};

