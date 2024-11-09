#include "ImporterFBX.h"
#include <iostream>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Component.h"

using namespace std;

//////////////////////////////////////////
#define FBX_FILE "Assets/halo2.fbx"
//////////////////////////////////////////

ImporterFBX::ImporterFBX() :Component() {}

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

    // Iterar sobre cada malla en la escena
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        mesh = scene->mMeshes[i];

        glBegin(GL_TRIANGLES);  // Inicia el modo de triángulos

        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
            face = mesh->mFaces[f];

            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                unsigned int index = face.mIndices[j];
                // Configuración de coordenadas de textura
                if (mesh->mTextureCoords[0]) {
                    aiVector3D uv = mesh->mTextureCoords[0][index];
                    glTexCoord2f(uv.x, 1.0f - uv.y);  // Cambia entre `uv.y` y `1.0f - uv.y` si es necesario
                }

                // Configuración de las coordenadas de vértice
                vertex = mesh->mVertices[index];
                glVertex3f(vertex.x, vertex.y, vertex.z);

                // Configuración de normales (para iluminación)
                if (mesh->HasNormals()) {
                    aiVector3D normal = mesh->mNormals[index];
                    glNormal3f(normal.x, normal.y, normal.z);
                }
            }
        }
        glEnd();  // Termina el modo de triángulos
    }

}

int ImporterFBX::draw_fbx(const char* file) {
    const aiScene* scene = load_fbx(file);
    if (!scene) {
        printf("Error al cargar el archivo FBX\n");
        return -1;
    }

    // Inicializar los contadores
    totalVertices = 0;
    totalFaces = 0;

    // Contar los vértices y caras
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        totalVertices += mesh->mNumVertices;
        totalFaces += mesh->mNumFaces;
    }

    render_fbx(scene);
    aiReleaseImport(scene);
    return 0;
}


