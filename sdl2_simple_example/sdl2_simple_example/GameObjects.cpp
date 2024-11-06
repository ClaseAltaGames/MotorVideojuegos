#include "GameObjects.h"

#include "ImporterFBX.h"
#include "Component.h"


GameObjects::GameObjects() : Component()
{
}

GameObjects::~GameObjects()
{
}

void GameObjects::Update()
{
	
}

//Component* GameObjects::CreateComponent(ComponentType type)
//{
//	Component* component = nullptr;
//
//	switch (type)
//	{
//	case ComponentType::TRANSFORM:
//		//component = new Transform();
//		break;
//	case ComponentType::MESH:
//		//component = new Mesh();
//		break;
//	case ComponentType::MATERIAL:
//		//component = new Material();
//		break;
//	case ComponentType::CAMERA:
//		//component = new Camera();
//		break;
//	case ComponentType::LIGHT:
//		//component = new Light();
//		break;
//	case ComponentType::SCRIPT:
//		//component = new Script();
//		break;
//	}
//
//	components.push_back(component);
//	return component;
//}



