#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include <glm/glm.hpp>  // Incluir GLM para usar glm::vec3
#include "Textures.h"
#include "ImporterFBX.h"
#include "DisplayFunc.h"

// Clase GameObject para gestionar componentes de un objeto 3D
class GameObject {
public:
    GameObject(const std::string& name);  // Constructor
    ~GameObject() {};  // Destructor

    // Métodos para añadir componentes al GameObject
    void AddTexture(const std::string& textureFile);  // Añadir textura
    void AddFBXModel(const std::string& modelFile);  // Añadir modelo FBX

    // Métodos para manipular las transformaciones
    void SetPosition(float x, float y, float z);
    void SetRotation(float x, float y, float z);
    void SetScale(float x, float y, float z);

    void Translate(float dx, float dy, float dz);
    void Rotate(float dx, float dy, float dz);
    void Scale(float dx, float dy, float dz);

    // Método para actualizar el objeto (cargar componentes, aplicar efectos, etc.)
    void Update();

    // Método para renderizar el objeto en la pantalla
    void Render();

    void DrawGameObjects(std::list<GameObject>* gameObjects);

private:
    std::string name;  // Nombre del objeto
    glm::vec3 position, rotation, scale;  // Transformaciones usando glm::vec3
    bool textureLoaded;  // Estado de la textura cargada

    Textures textures;  // Componente de textura
    ImporterFBX fbxModel;  // Componente de modelo 3D
};

#endif // __GAMEOBJECT_H__
