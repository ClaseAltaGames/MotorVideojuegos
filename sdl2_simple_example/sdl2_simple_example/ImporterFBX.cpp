#include "ImporterFBX.h"
#include <iostream>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

using namespace std;

//////////////////////////////////////////
#define FBX_FILE "Assets/halo2.fbx"
//////////////////////////////////////////

ImporterFBX::ImporterFBX() {}

ImporterFBX::~ImporterFBX() {}

const aiScene* ImporterFBX::load_fbx(const char* file) {
    const aiScene* scene = aiImportFile(file, aiProcess_Triangulate | aiProcess_GenNormals);
    if (!scene) {
        cerr << "Error al cargar el archivo: " << aiGetErrorString() << endl;
        return nullptr;
    }
    return scene;
}

void ImporterFBX::render_fbx(const aiScene* scene) {
    if (!scene) {
        cerr << "Error: escena no cargada" << endl;
        return;
    }
    //if (mesh->HasTextureCoords(0)) {  // Verifica que haya coordenadas de textura
    //    glEnable(GL_TEXTURE_2D);
    //    glBindTexture(GL_TEXTURE_2D, textureID);  // Usar tu `textureID`
    //}


    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        // Renderizar los v?rtices y las caras
        glBegin(GL_TRIANGLES);
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
            aiFace face = mesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                aiVector3D vertex = mesh->mVertices[face.mIndices[j]];
                glVertex3f(vertex.x, vertex.y, vertex.z);
                if (mesh->mTextureCoords[0]) {  // Verificar que haya coordenadas de textura
                    aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
                    glTexCoord2f(uv.x, uv.y);
                }
            }
        }
        glEnd();
    }
}

int ImporterFBX::draw_fbx(const char* file) {
    const aiScene* scene = load_fbx(file);
    if (!scene) {
        printf("Error al cargar el archivo FBX\n");
        return -1;
    }
    render_fbx(scene);
    aiReleaseImport(scene);
    return 0;
}

struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    Transform() : position(0.0f), rotation(0.0f), scale(1.0f) {}

    glm::mat4 getMatrix() const {
        glm::mat4 trans = glm::translate(glm::mat4(1.0f), position);
        trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        trans = glm::scale(trans, scale);
        return trans;
    }
};
struct Mesh {
    unsigned int vao, vbo;
    aiMesh* aiMeshData;

    Mesh(aiMesh* meshData) : aiMeshData(meshData) {
        // Cargar vértices, UVs y otras propiedades en VAO/VBO aquí
    }

    void render() {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, aiMeshData->mNumVertices);
        glBindVertexArray(0);
    }
};

struct Texture {
    unsigned int id;

    Texture(const std::string& filePath) {
        // Cargar la textura aquí usando stb_image o SDL_image
    }

    void bind() const {
        glBindTexture(GL_TEXTURE_2D, id);
    }
};
struct GameObject {
    Transform transform;
    Mesh* mesh;
    Texture* texture;

    GameObject(Mesh* m, Texture* t) : mesh(m), texture(t) {}

    void render() {
        // Aplicar transformación
        glm::mat4 modelMatrix = transform.getMatrix();
        //glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &modelMatrix[0][0]);

        // Enlazar textura
        texture->bind();

        // Renderizar malla
        mesh->render();
    }
};