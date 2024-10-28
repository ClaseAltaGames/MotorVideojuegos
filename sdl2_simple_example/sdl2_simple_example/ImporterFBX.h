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
    // Función auxiliar para cargar el archivo FBX
    const aiScene* load_fbx(const char* file);
    // Función auxiliar para renderizar la escena cargada
    void render_fbx(const aiScene* scene);
};