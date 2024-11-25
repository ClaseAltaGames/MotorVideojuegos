#pragma once
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include "Component.h"

class ImporterFBX : public Component {
public:
    ImporterFBX();
    ~ImporterFBX();

    // Load and render the FBX file
    int draw_fbx(const char* file);

    // Set rotation angles for the model
    void setRotation(float angleX, float angleY, float angleZ);

    // Setters for transformations (position and scale can also be controlled externally)
    void setPosition(float posX, float posY, float posZ);
    void setScale(float scaleX, float scaleY, float scaleZ);

    // Show ImGui controls for transformation
    void renderUI();

    int totalVertices = 3075;  // Total vertices, dynamically updated on load
    int totalFaces = 1025;     // Total faces, dynamically updated on load

private:
    // Internal function to load the FBX file
    const aiScene* load_fbx(const char* file);

    // Internal function to render the loaded scene
    void render_fbx(const aiScene* scene);

    // Update the transformation matrix
    void update_transformations();

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);   // Position of the model
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);   // Rotation (in degrees) for each axis
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);      // Scale of the model
    glm::mat4 transform = glm::mat4(1.0f);              // Transformation matrix combining position, rotation, and scale
};
extern ImporterFBX* importerFBX;
