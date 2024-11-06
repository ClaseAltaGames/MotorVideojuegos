#ifndef TEXTURES_H
#define TEXTURES_H

#include "Component.h"
#include <string>
#include <GL/glew.h>
#include <IL/il.h>
#define TEXTURE_FILE "Assets/Baker_house.png"

class Textures : public Component {
public:
    Textures();
    ~Textures();

    // Cargar la textura desde un archivo
    void LoadTexture(const char* filePath);

    // Dibujar la textura cargada
    void DrawTexture();

private:
    GLuint textureID;  // ID de la textura en OpenGL
    bool isTextureLoaded;  // Bandera para verificar si la textura ya está cargada
};

#endif // TEXTURES_H
