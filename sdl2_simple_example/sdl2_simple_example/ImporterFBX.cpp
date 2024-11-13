#include "ImporterFBX.h"
#include <iostream>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Component.h"
#include "imgui.h"

using namespace std;

ImporterFBX::ImporterFBX() : Component() {
    // Initialize position, rotation, and scale
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    transform = glm::mat4(1.0f);  // Identity matrix
}

ImporterFBX::~ImporterFBX() {}

const aiScene* ImporterFBX::load_fbx(const char* file) {
    const aiScene* scene = aiImportFile(file, aiProcess_Triangulate | aiProcess_GenNormals);
    if (!scene) {
        cerr << "Error loading file: " << aiGetErrorString() << endl;
        return nullptr;
    }
    return scene;
}

void ImporterFBX::render_fbx(const aiScene* scene) {
    if (!scene) {
        cerr << "Error: scene not loaded" << endl;
        return;
    }

    glPushMatrix();
    glMultMatrixf(glm::value_ptr(transform));  // Apply combined transformation matrix

    // Render each mesh in the scene
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        glBegin(GL_TRIANGLES);
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
            aiFace face = mesh->mFaces[f];

            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                unsigned int index = face.mIndices[j];

                if (mesh->mTextureCoords[0]) {
                    aiVector3D uv = mesh->mTextureCoords[0][index];
                    glTexCoord2f(uv.x, 1.0f - uv.y);
                }

                if (mesh->HasNormals()) {
                    aiVector3D normal = mesh->mNormals[index];
                    glNormal3f(normal.x, normal.y, normal.z);
                }

                aiVector3D vertex = mesh->mVertices[index];
                glVertex3f(vertex.x, vertex.y, vertex.z);
            }
        }
        glEnd();
    }

    glPopMatrix();
}

int ImporterFBX::draw_fbx(const char* file) {
    const aiScene* scene = load_fbx(file);
    if (!scene) {
        printf("Error loading FBX file\n");
        return -1;
    }

    totalVertices = 0;
    totalFaces = 0;

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        totalVertices += mesh->mNumVertices;
        totalFaces += mesh->mNumFaces;
    }

    render_fbx(scene);
    aiReleaseImport(scene);
    return 0;
}

void ImporterFBX::update_transformations() {
    // Construct transformation matrix
    transform = glm::mat4(1.0f); // Reset to identity
    transform = glm::translate(transform, position);
    transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, scale);
}

void ImporterFBX::renderUI() {
    // ImGui sliders for position
    if (ImGui::SliderFloat3("Position", glm::value_ptr(position), -10.0f, 10.0f)) {
        update_transformations();
    }

    // ImGui sliders for rotation
    if (ImGui::SliderFloat3("Rotation", glm::value_ptr(rotation), -180.0f, 180.0f)) {
        update_transformations();
    }

    // ImGui sliders for scale
    if (ImGui::SliderFloat3("Scale", glm::value_ptr(scale), 0.1f, 10.0f)) {
        update_transformations();
    }
}

// Setters for rotation, scale, and position
void ImporterFBX::setRotation(float angleX, float angleY, float angleZ) {
    rotation = glm::vec3(angleX, angleY, angleZ);
    update_transformations();
}

void ImporterFBX::setScale(float scaleX, float scaleY, float scaleZ) {
    scale = glm::vec3(scaleX, scaleY, scaleZ);
    update_transformations();
}

void ImporterFBX::setPosition(float posX, float posY, float posZ) {
    position = glm::vec3(posX, posY, posZ);
    update_transformations();
}
