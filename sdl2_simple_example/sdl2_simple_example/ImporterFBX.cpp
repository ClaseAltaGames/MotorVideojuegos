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