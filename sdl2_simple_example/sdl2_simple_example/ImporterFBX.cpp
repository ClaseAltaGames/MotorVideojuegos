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

// Function to load FBX data from a file
int ImporterFBX::Load_FBX(const char* file, std::vector<std::vector<aiVector3D>>& vertices, std::vector<std::vector<aiVector3D>>& uvs) {
    // Import the FBX file using Assimp
    const struct aiScene* scene = aiImportFile(file, aiProcess_Triangulate);
    if (!scene) {
        fprintf(stderr, "Error en carregar el fitxer: %s\n", aiGetErrorString());
        return -1;
    }

    // Loop through all the meshes in the scene
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        std::vector<aiVector3D> meshVertices;
        std::vector<aiVector3D> meshUVs;

        // Vértices
        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            meshVertices.push_back(mesh->mVertices[v]);
        }

        // UVs: Check if the mesh has texture coordinates
        if (mesh->HasTextureCoords(0)) {
            for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
                aiFace face = mesh->mFaces[f];
                for (unsigned int j = 0; j < face.mNumIndices; j++) {
                    meshUVs.push_back(mesh->mTextureCoords[0][face.mIndices[j]]);
                }
            }
        }

        // Store the mesh data
        vertices.push_back(meshVertices);
        uvs.push_back(meshUVs);
    }

    // Release the imported scene
    aiReleaseImport(scene);
    return 0;
}

// Function to draw the loaded FBX data
void ImporterFBX::Draw_FBX(const std::vector<std::vector<aiVector3D>>& vertices, const std::vector<std::vector<aiVector3D>>& uvs) {
    // Loop through each mesh's vertex data
    for (size_t i = 0; i < vertices.size(); i++) {
        glBegin(GL_TRIANGLES);  // Start drawing triangles

        // Loop through each face's vertex data
        for (size_t j = 0; j < vertices[i].size(); j++) {
            const aiVector3D& vertex = vertices[i][j];
            glVertex3f(vertex.x, vertex.y, vertex.z);  // Set vertex position

            // Check if the UVs for this mesh exist and set the texture coordinates
            if (i < uvs.size() && j < uvs[i].size()) {
                const aiVector3D& uv = uvs[i][j];
                glTexCoord2f(uv.x, uv.y);  // Set texture coordinates
            }
        }

        glEnd();  // End drawing triangles
    }
}
