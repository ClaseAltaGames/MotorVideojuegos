#include "ImporterFBX.h"
#include <iostream>
#include <GL/glew.h>


//////////////////////////////////////////
#define FBX_FILE "Assets/halo2.fbx"
//////////////////////////////////////////
ImporterFBX::ImporterFBX()
{    
   
}

ImporterFBX::~ImporterFBX()
{

}

int ImporterFBX::Load_FBX(const char* file, std::vector<std::vector<aiVector3D>>& vertices, std::vector<std::vector<aiVector3D>>& uvs) {
    const struct aiScene* scene = aiImportFile(file, aiProcess_Triangulate);
    if (!scene) {
        fprintf(stderr, "Error en carregar el fitxer: %s\n", aiGetErrorString());
        return -1;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        std::vector<aiVector3D> meshVertices;
        std::vector<aiVector3D> meshUVs;

        // Vértices
        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            meshVertices.push_back(mesh->mVertices[v]);
        }

        // Índices de caras (para obtener las UV correspondientes)
        if (mesh->HasTextureCoords(0)) {
            for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
                aiFace face = mesh->mFaces[f];
                for (unsigned int j = 0; j < face.mNumIndices; j++) {
                    meshUVs.push_back(mesh->mTextureCoords[0][face.mIndices[j]]);
                }
            }
        }

        vertices.push_back(meshVertices);
        uvs.push_back(meshUVs);
    }

    aiReleaseImport(scene);
    return 0;
}

void ImporterFBX::Draw_FBX(const std::vector<std::vector<aiVector3D>>& vertices, const std::vector<std::vector<aiVector3D>>& uvs) {
    for (size_t i = 0; i < vertices.size(); i++) {
        glBegin(GL_TRIANGLES);

        for (size_t j = 0; j < vertices[i].size(); j++) {
            const aiVector3D& vertex = vertices[i][j];
            glVertex3f(vertex.x, vertex.y, vertex.z);

            if (i < uvs.size() && j < uvs[i].size()) {
                const aiVector3D& uv = uvs[i][j];
                glTexCoord2f(uv.x, uv.y);
            }
        }

        glEnd();
    }
}

