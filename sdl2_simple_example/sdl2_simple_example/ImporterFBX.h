#pragma once
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include <iostream>

class ImporterFBX {
public:
    ImporterFBX();
    ~ImporterFBX();

    // Cargar y dibujar archivo FBX
    int draw_fbx(const char* file);

private:
    // Funci�n auxiliar para cargar el archivo FBX
    const aiScene* load_fbx(const char* file);
    // Funci�n auxiliar para renderizar la escena cargada
    void render_fbx(const aiScene* scene);
};