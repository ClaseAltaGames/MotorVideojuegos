#include "GameObjects.h"

// Constructor de GameObject
GameObject::GameObject(const std::string& name)
    : name(name), position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), textureLoaded(false) {}

// Métodos para añadir componentes
void GameObject::AddTexture(const std::string& textureFile) {
    textures.LoadTexture(textureFile.c_str());
    textureLoaded = true;
}

void GameObject::AddFBXModel(const std::string& modelFile) {
    fbxModel.load_fbx(modelFile.c_str());  // Asegúrate de que ImporterFBX tenga el método LoadModel
}

// Métodos para manipular las transformaciones
void GameObject::SetPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

void GameObject::SetRotation(float x, float y, float z) {
    rotation = glm::vec3(x, y, z);
}

void GameObject::SetScale(float x, float y, float z) {
    scale = glm::vec3(x, y, z);
}

void GameObject::Translate(float dx, float dy, float dz) {
    position += glm::vec3(dx, dy, dz);
}

void GameObject::Rotate(float dx, float dy, float dz) {
    rotation += glm::vec3(dx, dy, dz);
}

void GameObject::Scale(float dx, float dy, float dz) {
    scale *= glm::vec3(dx, dy, dz);
}

// Método para actualizar el objeto (cargar componentes, aplicar efectos, etc.)
void GameObject::Update() {
    if (textureLoaded) {
        textures.DrawTexture();
    }
    fbxModel.draw_fbx(displayFunc->currentFBXFile.c_str());  // Asegúrate de que ImporterFBX tenga un método `Draw`
}

// Método para renderizar el objeto en la pantalla
void GameObject::Render() {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);  // Usar los valores de position, rotation y scale
    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
    glScalef(scale.x, scale.y, scale.z);  // Usar los valores de position, rotation y scale

    Update();  // Actualizar estado del objeto
    glPopMatrix();
}

void GameObject::DrawGameObjects(std::list<GameObject>* gameObjects) {
	for (auto& gameObject : *gameObjects) {
		gameObject.Render();
	}
}
