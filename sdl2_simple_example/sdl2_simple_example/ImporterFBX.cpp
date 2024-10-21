#include "ImporterFBX.h"
#include <iostream>
#include <GL/glew.h>

//////////////////////////////////////////
#define FBX_FILE "Assets/cube.fbx"
//////////////////////////////////////////
ImporterFBX::ImporterFBX()
{
}

ImporterFBX::~ImporterFBX()
{
}

int ImporterFBX::Draw_FBX(const char* file) {
    const struct aiScene* scene = aiImportFile(file, aiProcess_Triangulate);
    if (!scene) {
        fprintf(stderr, "Error en carregar el fitxer: %s\n", aiGetErrorString());
        return -1;
    }
    //printf("Numero de malles: %u\n", scene->mNumMeshes);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        //actualizar la posicion de la camara
        aiMesh* mesh = scene->mMeshes[i];
        // Vèrtexs
        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            aiVector3D vertex = mesh->mVertices[v];

        }
        // Índexs de triangles (3 per triangle)
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {

            aiFace face = mesh->mFaces[f];


            for (unsigned int j = 0; j < face.mNumIndices; j++) {

            }

        }
        //pasar los arrays a openGL
        glBegin(GL_TRIANGLES);
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
            aiFace face = mesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                aiVector3D vertex = mesh->mVertices[face.mIndices[j]];
                glVertex3f(vertex.x, vertex.y, vertex.z);
                aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
                glTexCoord2f(uv.x, uv.y);
            }
        }
        glEnd();
    }
    aiReleaseImport(scene);
    return 0;
}
bool ImporterFBX::Start() {
	return true;
}
bool ImporterFBX::Update() {
    Draw_FBX(FBX_FILE);
	return true;
}
bool ImporterFBX::CleanUp() {
	return true;
}
