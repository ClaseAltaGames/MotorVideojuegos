#pragma once
#include <string>
#include <glm/glm.hpp>
#include "ImporterFBX.h"
#include "BasicForms.h"
#include "Camera.h"
#include "Textures.h"
#include "Gizmo.h"

#define FBX_FILE "Assets/BakerHouse.fbx"
#define TEXTURE_FILE "Assets/Baker_house.dds"

using namespace std;

class DisplayFunc {
public:
    DisplayFunc();
    ~DisplayFunc();

    // Main display function to render all components
    void DisplayALL();

    // Draws a grid for the scene
    void drawGrid(float size, int divisions);

    // Sets the active basic form to display
    void setActiveObject(const std::string& objectName);

    // Flags for toggling visibility of different objects
    bool cubeActive = false;
    bool pyramidActive = false;
    bool sphereActive = false;
    bool cylinderActive = false;
    bool textureLoaded = false;

	bool translateActive = false;  
	bool scaleActive = false;
	bool rotateActive = false;

    // File paths for the FBX model and texture
    string currentFBXFile = FBX_FILE;
    string currentTextureFile = TEXTURE_FILE;

    // Transformation variables for the FBX model
    glm::vec3 fbxPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 fbxRotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 fbxScale = glm::vec3(1.0f, 1.0f, 1.0f);

    // Method to render the ImGui-based gizmo UI
    void renderGizmoUI();
};

